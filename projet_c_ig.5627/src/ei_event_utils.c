#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ei_event_utils.h"
#include "ei_toplevel.h"
#include "ei_application.h"
#include "ei_geometrymanager.h"

ei_bool_t drawing = EI_FALSE;
ei_bool_t destroy = EI_FALSE;
ei_linked_event_t* listed_events;

ei_linked_event_t* get_list_events (){
        return listed_events;
}

// Animation bouton
ei_button_t* sunken_button;

// Déplacement toplevel
ei_toplevel_t* moving_toplevel;
ei_point_t mouse_pos;

//resize toplevel
ei_toplevel_t* resized_toplevel;
ei_point_t new_size;
//ei_surface_t picking = ei_app_pick_surface();

void set_list_events (ei_linked_event_t* list) {
        listed_events = list;
}

//faut un fonction pour def les events /bind de base, genre une init qu'on va appeller dans le ei_app_create
void ei_init_list_events (){

        // Animation de pression sur bouton
        ei_bind(ei_ev_mouse_move, NULL, "all", (ei_callback_t)&getoutofbutton_animation, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "button", (ei_callback_t)&pressbutton_animation, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "button", (ei_callback_t)&unpressbutton_animation, NULL);

        // Déplacement toplevel
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", (ei_callback_t)&click_toplevel_header, NULL);
        ei_bind(ei_ev_mouse_move, NULL, "all", (ei_callback_t)&move_toplevel, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "toplevel", (ei_callback_t)&unclick_toplevel, NULL);

        //resize_toplevel
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", (ei_callback_t)&click_resize_toplevel, NULL);
        ei_bind(ei_ev_mouse_move, NULL, "all", (ei_callback_t)&resizing_toplevel, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "toplevel", (ei_callback_t)&stop_resize, NULL);
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
        // if(strcmp(widget->wclass->name,"toplevel") == 0) {
        //         ei_widget_t* close_button = ((ei_widget_t*)(((ei_toplevel_t*)widget)->close_button));
        //         if(pick_id == close_button->pick_id){
        //                 return close_button;
        //         }
        // }
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
ei_bool_t click_toplevel_header(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if (event->param.mouse.where.y < widget->content_rect->top_left.y) {
                moving_toplevel = (ei_toplevel_t*)widget;
                mouse_pos = event->param.mouse.where;
        }
        return EI_FALSE;
}

ei_bool_t move_toplevel(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if (moving_toplevel) {
                ei_widget_t* moving_widget = (ei_widget_t*)moving_toplevel;
                ei_linked_rect_t** rect_list = get_rect_list();
                ei_rect_t intersection;
                ei_intersection_rectangle(&moving_widget->parent->content_rect ,&moving_widget->screen_location, &intersection);
                rect_list_add(rect_list, intersection);
                moving_widget->screen_location.top_left.x += event->param.mouse.where.x - mouse_pos.x;
                moving_widget->screen_location.top_left.y += event->param.mouse.where.y - mouse_pos.y;
                // ei_place(moving_widget, NULL, &(moving_widget->screen_location.top_left.x), &(moving_widget->screen_location.top_left.y), NULL, NULL, NULL, NULL, NULL, NULL);
                mouse_pos = event->param.mouse.where;
                ei_widget_t* current = moving_widget->children_head;
                while (current) {
                        current->geom_params->manager->runfunc(current);
                        ei_widget_t* current_bro = current->next_sibling;
                        while (current_bro) {
                                current_bro->geom_params->manager->runfunc(current_bro);
                                current_bro = current_bro->next_sibling;
                        }
                        current = current->children_head;
                }
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

ei_bool_t click_resize_toplevel(ei_widget_t* widget, struct ei_event_t* event, void* user_param){
        if ((widget->screen_location.top_left.y + widget->screen_location.size.height - 4 * ((ei_toplevel_t*)widget)->border_width) < event->param.mouse.where.y &&  event->param.mouse.where.y < (widget->screen_location.top_left.y + widget->screen_location.size.height) &&\
        (widget->screen_location.top_left.x + widget->screen_location.size.width - 4 * ((ei_toplevel_t*)widget)->border_width) < event->param.mouse.where.x && event->param.mouse.where.x < (widget->screen_location.top_left.x + widget->screen_location.size.width)) {
                new_size = event->param.mouse.where;
                resized_toplevel = (ei_toplevel_t*)widget;
        }
        return EI_FALSE;
}

ei_bool_t resizing_toplevel(ei_widget_t* widget, struct ei_event_t* event, void* user_param){
        if(resized_toplevel){
                ei_linked_rect_t** rect_list = get_rect_list();
                ei_widget_t* resized_widget = (ei_widget_t*)resized_toplevel;
                rect_list_add(rect_list, widget->content_rect);
                if(ei_axis_none){
                        resized_toplevel = NULL;
                } else if(ei_axis_both) {
                        resized_widget->content_rect->size.width -= new_size.x - event->param.mouse.where.x;
                        resized_widget->content_rect->size.height -= new_size.y - event->param.mouse.where.y;
                        new_size = event->param.mouse.where;
                } else if(ei_axis_x) {
                        resized_widget->content_rect->size.width -= new_size.x - event->param.mouse.where.x;
                        new_size = event->param.mouse.where;
                } else {
                        resized_widget->content_rect->size.height -= new_size.y - event->param.mouse.where.y;
                        new_size = event->param.mouse.where;
                }
                //ei_place(resized_widget, NULL, &(resized_widget->screen_location.top_left.x), &(resized_widget->screen_location.top_left.y), NULL, NULL, NULL, NULL, NULL, NULL);
                rect_list_add(rect_list, resized_widget->content_rect);
                ei_widget_t* current = resized_widget->children_head;
                while (current) {
                        current->geom_params->manager->runfunc(current);
                        ei_widget_t* current_bro = current->next_sibling;
                        while (current_bro) {
                                current_bro->geom_params->manager->runfunc(current_bro);
                                current_bro = current_bro->next_sibling;
                        }
                        current = current->children_head;
                }
        }
        return EI_FALSE;
}

ei_bool_t stop_resize(ei_widget_t* widget, struct ei_event_t* event, void* user_param){
        if (resized_toplevel) {
                resized_toplevel = NULL;
        }
        return EI_FALSE;
}
