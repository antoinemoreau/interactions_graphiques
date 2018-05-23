#include "ei_event_utils.h"
#include "ei_event.h"
#include <stdlib.h>
#include "ei_button.h"
#include "ei_application.h"

ei_linked_event_t* listed_events;

ei_linked_event_t* list_events (){
        return listed_events;
}
//faut un fonction pour def les events /bind de base, genre une init qu'on va appeller dans le ei_app_create
void ei_init_list_events (){
        // Animation de pression sur bouton
        ei_bind(ei_ev_mouse_buttondown, NULL, "button", (ei_callback_t)&pressbutton_animation, NULL);
}
ei_linked_event_t* find_event(ei_widget_t* widget, ei_eventtype_t eventtype, ei_tag_t tag, ei_callback_t callback){
        ei_linked_event_t* current = list_events();
        while(current){
                if(current->widget == widget && current->eventtype == eventtype && current->tag == tag && current->callback == callback){
                        return current;
                }else{
                        current = current->next;
                }
        }
}
ei_widget_t* ei_pick_widget(int pick_id, ei_widget_t* widget){
        ei_widget_t* picked_widget = NULL;
        if (pick_id == widget->pick_id) {
                return widget;
        }else{
                ei_widget_t* child = widget->children_head;
                while (child) {
                        ei_widget_t* pick = ei_pick_widget(pick_id, child);
                        if(pick){
                                picked_widget = pick;
                        }
                        child = child->next_sibling;
                }
        }
        return picked_widget;
}

// int     widget_under_mouse(ei_mouse_event_t mouse){
//
// }

ei_bool_t pressbutton_animation(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        ei_button_t* button = (ei_button_t*)widget;
        button->relief = ei_relief_sunken;
}

// TODO : Ecrire une autre fonction pour raised le bouton quand on relache
