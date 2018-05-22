#include "ei_event_utils.h"
#include "ei_event.h"

extern ei_linked_event_t* listed_event;

ei_linked_event_t* list_events(){
        return listed_events;
}
//faut un fonction pour def les events /bind de base, genre une init qu'on va appeller dans le ei_app_create
void ei_init_list_events(){
        ei_bind(ei_ev_mouse_buttondown, );
}
