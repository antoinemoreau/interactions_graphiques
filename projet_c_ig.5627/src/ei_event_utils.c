#include "ei_event_utils.h"
#include <stdlib.h>
#include "ei_button.h"
#include "ei_application.h"
#include <stdio.h>
#include <stdint.h>

ei_linked_event_t* listed_events;

ei_linked_event_t* get_list_events (){
        return listed_events;
}
ei_button_t* sunken_button;

void set_list_events (ei_linked_event_t* list) {
        listed_events = list;
}
//faut un fonction pour def les events /bind de base, genre une init qu'on va appeller dans le ei_app_create
void ei_init_list_events (){
        // Animation de pression sur bouton
        ei_bind(ei_ev_mouse_buttondown, NULL, "button", (ei_callback_t)&pressbutton_animation, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "all", (ei_callback_t)&unpressbutton_animation, NULL);
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
ei_widget_t* ei_pick_widget(uint32_t pick_id, ei_widget_t* widget){
        ei_widget_t* picked_widget = NULL;
        if (pick_id == widget->pick_id) {
                return widget;
        }
        // for (ei_widget_t* child = widget->children_head; child != NULL; child = child->next_sibling) {
        //         ei_pick_widget(pick_id, child);
        // }
        ei_widget_t* child = widget->children_head;
        while (child) {
                ei_widget_t* pick = ei_pick_widget(pick_id, child);
                if(pick){
                        picked_widget = pick;
                }
                child = child->next_sibling;
        }
        return picked_widget;
}

ei_bool_t unpressbutton_animation(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if(event->type == ei_ev_mouse_buttonup){
                if(sunken_button){
                        ei_button_t* button = sunken_button;
                        button->relief = ei_relief_raised;
                        return EI_TRUE;
                }
        }
        return EI_FALSE;
}

ei_bool_t pressbutton_animation(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if(event->type == ei_ev_mouse_buttondown){
                sunken_button = (ei_button_t*)widget;
                sunken_button->relief = ei_relief_sunken;
                //return button_press(widget, event, user_param);
                return EI_FALSE;
        }
        return EI_FALSE;
}
