#include "ei_types.h"
#include "ei_widget.h"
#include "ei_event.h"
#include "ei_button.h"
#include "ei_event_utils.h"
#include <stdlib.h>


void		ei_bind			(ei_eventtype_t		eventtype,
					 ei_widget_t*		widget,
					 ei_tag_t		tag,
					 ei_callback_t		callback,
					 void*			user_param){
	ei_linked_event_t* list_of_events = list_events();
	ei_linked_event_t* event = calloc(1,sizeof(ei_linked_event_t));
	event->eventtype = eventtype;
	event->widget = widget;
	event->tag = tag;
	event->callback = callback;
	event->user_param = user_param;
	event->next = list_of_events;
	list_of_events = event;
}

void		ei_unbind		(ei_eventtype_t		eventtype,
					 ei_widget_t*		widget,
					 ei_tag_t		tag,
					 ei_callback_t		callback,
					 void*			user_param){

}
