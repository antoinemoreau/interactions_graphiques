#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ei_event_utils.h"
#include "ei_toplevel.h"
#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "ei_application_utils.h"
#include "ei_draw_utils.h"

ei_bool_t drawing = EI_FALSE;
ei_bool_t destroy = EI_FALSE;
ei_linked_event_t* listed_events;

ei_linked_event_t* get_list_events () {
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

void set_list_events (ei_linked_event_t* list) {
        listed_events = list;
}

void free_list_events () {
        ei_linked_event_t* del = listed_events;
        while (listed_events != NULL) {
                listed_events = listed_events->next;
                free(del);
                del = listed_events;
        }
}

void ei_init_list_events (){

        // Animation de pression sur bouton
        ei_bind(ei_ev_mouse_move, NULL, "all", (ei_callback_t)&getoutofbutton_animation, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "button", (ei_callback_t)&pressbutton_animation, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "button", (ei_callback_t)&unpressbutton_animation, NULL);

        // Déplacement toplevel
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", (ei_callback_t)&click_toplevel_header, NULL);
        ei_bind(ei_ev_mouse_move, NULL, "all", (ei_callback_t)&move_toplevel, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "toplevel", (ei_callback_t)&unclick_toplevel, NULL);

        // resize_toplevel
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", (ei_callback_t)&click_resize_toplevel, NULL);
        ei_bind(ei_ev_mouse_move, NULL, "all", (ei_callback_t)&resizing_toplevel, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "toplevel", (ei_callback_t)&stop_resize, NULL);

        // Faire passer un widget au premier plan
        //ei_bind(ei_ev_mouse_buttondown, NULL, "all", (ei_callback_t)&move_foreground, NULL);
}

ei_linked_event_t* find_event(ei_widget_t* widget, ei_eventtype_t eventtype, ei_tag_t tag, ei_callback_t callback) {
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

ei_widget_t* ei_find_widget(uint32_t pick_id, ei_widget_t* widget) {
        if (pick_id == widget->pick_id) {
                return widget;
        }
        ei_widget_t* child = widget->children_head;
        while (child) {
                ei_widget_t* pick = ei_find_widget(pick_id, child);
                if(pick){
                        return pick;
                }
                child = child->next_sibling;
        }
        return NULL;
}
//fonctions de callbacks des boutons
ei_bool_t unpressbutton_animation(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if(sunken_button && event->param.mouse.button_number == 1){
                sunken_button->relief = ei_relief_raised;
                sunken_button = NULL;
        }
        return EI_FALSE;
}

ei_bool_t pressbutton_animation(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if (event->param.mouse.button_number == 1) {
                sunken_button = (ei_button_t*)widget;
                sunken_button->relief = ei_relief_sunken;
        }
        return EI_FALSE;
}

ei_bool_t getoutofbutton_animation(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if(sunken_button && widget->pick_id != sunken_button->widget.pick_id){
                sunken_button->relief = ei_relief_raised;
                sunken_button = NULL;
        }
        return EI_FALSE;
}

//fonctions de callback des toplevels
ei_bool_t click_toplevel_header(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if (event->param.mouse.where.y < widget->content_rect->top_left.y && event->param.mouse.button_number == 1) {
                moving_toplevel = (ei_toplevel_t*)widget;
                mouse_pos = event->param.mouse.where;
        }
        return EI_FALSE;
}

ei_bool_t move_toplevel(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if (moving_toplevel) {
                ei_widget_t* moving_widget = (ei_widget_t*)moving_toplevel;
                ei_linked_rect_t** rect_list = get_rect_list();
                ei_rect_t intersection1;
                ei_intersection_rectangle(moving_widget->parent->content_rect ,&moving_widget->screen_location, &intersection1);
                rect_list_add(rect_list, intersection1);
                moving_widget->screen_location.top_left.x += event->param.mouse.where.x - mouse_pos.x;
                moving_widget->screen_location.top_left.y += event->param.mouse.where.y - mouse_pos.y;
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
                ei_rect_t intersection2;
                ei_intersection_rectangle(moving_widget->parent->content_rect, &moving_widget->screen_location, &intersection2);
                rect_list_add(rect_list, intersection2);
        }
        return EI_FALSE;
}

ei_bool_t unclick_toplevel(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if (moving_toplevel && event->param.mouse.button_number == 1) {
                moving_toplevel = NULL;
        }
        return EI_FALSE;
}

ei_bool_t click_resize_toplevel(ei_widget_t* widget, struct ei_event_t* event, void* user_param){
        if ((widget->screen_location.top_left.y + widget->screen_location.size.height - \
                4 * ((ei_toplevel_t*)widget)->border_width) < event->param.mouse.where.y \
                &&  event->param.mouse.where.y < (widget->screen_location.top_left.y + \
                widget->screen_location.size.height) && (widget->screen_location.top_left.x \
                + widget->screen_location.size.width - 4 * ((ei_toplevel_t*)widget)->border_width) \
                < event->param.mouse.where.x && event->param.mouse.where.x < \
                (widget->screen_location.top_left.x + widget->screen_location.size.width) \
                && event->param.mouse.button_number == 1) {

                new_size = event->param.mouse.where;
                resized_toplevel = (ei_toplevel_t*)widget;
        }
        return EI_FALSE;
}

ei_bool_t resizing_toplevel(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if (resized_toplevel) {
                ei_linked_rect_t** rect_list = get_rect_list();
                ei_widget_t* resized_widget = (ei_widget_t*)resized_toplevel;
                ei_rect_t intersection1;
                ei_intersection_rectangle(resized_widget->parent->content_rect ,&resized_widget->screen_location, &intersection1);
                rect_list_add(rect_list, intersection1);
                int diff_x = resized_widget->screen_location.size.width - new_size.x + event->param.mouse.where.x;
                int diff_y = resized_widget->screen_location.size.height - new_size.y + event->param.mouse.where.y;
                if(diff_x > resized_toplevel->border_width && diff_y > 2 * resized_toplevel->border_width){
                        if (ei_axis_none) {
                                resized_toplevel = NULL;
                        } else if(ei_axis_both) {
                                resized_widget->screen_location.size.width -= new_size.x - event->param.mouse.where.x;
                                resized_widget->screen_location.size.height -= new_size.y - event->param.mouse.where.y;
                                new_size = event->param.mouse.where;
                        } else if(ei_axis_x) {
                                resized_widget->screen_location.size.width -= new_size.x - event->param.mouse.where.x;
                                new_size = event->param.mouse.where;
                        } else {
                                resized_widget->screen_location.size.height -= new_size.y - event->param.mouse.where.y;
                                new_size = event->param.mouse.where;
                        }
                        
                        ei_rect_t intersection2;
                        ei_intersection_rectangle(resized_widget->parent->content_rect ,&resized_widget->screen_location, &intersection2);
                        rect_list_add(rect_list, intersection2);
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
        }
        return EI_FALSE;
}

ei_bool_t stop_resize(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        if (resized_toplevel) {
                resized_toplevel = NULL;
        }
        return EI_FALSE;
}

ei_bool_t move_foreground(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        // On fait passer le widget et tous ses ancêtres à la fin de leur
        // listes de frères respectives

        // ei_widget_t* previous = NULL;
        // ei_bool_t is_toplevel = strcmp(widget->wclass->name, "toplevel") == 0);

        // if (!widget->parent) {
        //         return EI_FALSE;
        // }

        // // Si il est déjà en queue on passe directement au père
        // if (!widget->next_sibling || (is_toplevel && widget->next_)

        // // On passe par le parent et on itère sur le fils jusqu'a trouver le fils précédent
        
        // while()

        // if (!widget->next_sibling) {
        //         move_foreground(widget->parent, event, user_param);
        // } else ( {

        // }

        
}
