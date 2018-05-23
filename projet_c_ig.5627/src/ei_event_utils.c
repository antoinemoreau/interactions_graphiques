#include "ei_event_utils.h"
#include "ei_event.h"
#include <stdlib.h>
#include "ei_button.h"

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

ei_bool_t pressbutton_animation(ei_widget_t* widget, struct ei_event_t* event, void* user_param) {
        ei_button_t* button = (ei_button_t*)widget;
        button->relief = ei_relief_sunken;
}

// TODO : Ecrire une autre fonction pour raised le bouton quand on relache
