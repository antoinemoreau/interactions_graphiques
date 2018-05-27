#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ei_event_utils.h"
#include "ei_toplevel.h"
#include "ei_application.h"

ei_bool_t drawing = EI_FALSE;
ei_linked_event_t* listed_events;

ei_linked_event_t* get_list_events (){
        return listed_events;
}

// Animation bouton
ei_button_t* sunken_button;

// Déplacement toplevel
ei_toplevel_t* moving_toplevel;
ei_point_t mouse_pos;

void set_list_events (ei_linked_event_t* list) {
        listed_events = list;
}

//faut un fonction pour def les events /bind de base, genre une init qu'on va appeller dans le ei_app_create
void ei_init_list_events (){

        // Animation de pression sur bouton
        ei_bind(ei_ev_mouse_move, NULL, "all", (ei_callback_t)&getoutofbutton_animation, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "button", (ei_callback_t)&pressbutton_animation, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "button", (ei_callback_t)&unpressbutton_animation, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "button", (ei_callback_t)&closing, NULL); // on peut peut-etre le faire à la création du widget

        // Déplacement toplevel
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", (ei_callback_t)&click_toplevel_header, NULL);
        ei_bind(ei_ev_mouse_move, NULL, "toplevel", (ei_callback_t)&move_toplevel, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "toplevel", (ei_callback_t)&unclick_toplevel, NULL);
}

ei_linked_event_t* find_event(ei_widget_t* widget, ei_eventtype_t eventtype, ei_tag_t tag, ei_callback_t callback){
        ei_linked_event_t* current = get_list_events();
        while(current){
                if(current->widget == widget && current->eventtype == eventtype && current->tag == tag && current->callback == callback){
                        return current;
                }else{
                        current = current->next;
                }
        }
        fprintf(stderr, "cet evenement n'est pas encore traité \n");
        exit(1);
}

ei_widget_t* ei_find_widget(uint32_t pick_id, ei_widget_t* widget){
        ei_widget_t* picked_widget = NULL;
        if (pick_id == widget->pick_id) {
                return widget;
        }
        if(strcmp(widget->wclass->name,"toplevel") == 0) {
                ei_widget_t* close_button = ((ei_widget_t*)(((ei_toplevel_t*)widget)->close_button));
                if(pick_id == close_button->pick_id){
                        return close_button;
                }
        }
        ei_widget_t* child = widget->children_head;
        while (child) {
                ei_widget_t* pick = ei_find_widget(pick_id, child);
                if(pick){
                        picked_widget = pick;
                        return picked_widget;
                }
                child = child->next_sibling;
        }
}
//fonctions de callbacks des boutons
ei_bool_t unpressbutton_animation(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if(sunken_button){
                //ei_button_t* button = sunken_button;
                sunken_button->relief = ei_relief_raised;
                sunken_button = NULL;
                drawing = EI_TRUE;
                // return EI_TRUE
        }
        return EI_FALSE;
}

ei_bool_t pressbutton_animation(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        sunken_button = (ei_button_t*)widget;
        sunken_button->relief = ei_relief_sunken;
        drawing = EI_TRUE;
        return EI_FALSE;
}

ei_bool_t getoutofbutton_animation(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if(sunken_button && widget->pick_id != sunken_button->widget.pick_id){
                return unpressbutton_animation(widget, event, user_param);
        }
        return EI_FALSE;
}
//fonctions de callback des toplevels
ei_bool_t closing(ei_widget_t* widget, ei_event_t* event, void* user_param){
        if(widget->pick_id == (widget->parent->pick_id + 1) && strcmp(widget->parent->wclass->name,"toplevel") == 0){
                //on peut faire l'inverse et l'appeller dans pressbutton
                pressbutton_animation(widget, event, user_param);
                //faut appeller destroyer widget
                printf("FeRmEtuReUh 2 la feunaitre !\n");
                drawing = EI_TRUE;

        }
        return EI_FALSE;
}

ei_bool_t click_toplevel_header(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if (event->param.mouse.where.y < widget->content_rect->top_left.y) {
                moving_toplevel = (ei_toplevel_t*)widget;
                mouse_pos = event->param.mouse.where;
        }
        return EI_FALSE;
}

ei_bool_t move_toplevel(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if (moving_toplevel) {
                ei_linked_rect_t** rect_list = get_rect_list();
                rect_list_add(rect_list, widget->parent->screen_location);

                widget->screen_location.top_left.x += event->param.mouse.where.x - mouse_pos.x;
                widget->screen_location.top_left.y += event->param.mouse.where.y - mouse_pos.y;
                mouse_pos = event->param.mouse.where;
                // Il doit surement falloir faire un geomnotify ici pour mettre a jout la position des enfants.
                //ou une boucle de parcours des enfants et on met à jour leur positions non ?

                drawing = EI_TRUE;
        }
        return EI_FALSE;
}

ei_bool_t unclick_toplevel(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if (moving_toplevel) {
                moving_toplevel = NULL;
        }
        return EI_FALSE;
}
