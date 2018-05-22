#include "ei_types.h"
#include "ei_widget.h"
#include "ei_event.h"
#include "ei_button.h"


void		ei_bind			(ei_eventtype_t		eventtype,
					 ei_widget_t*		widget,
					 ei_tag_t		tag,
					 ei_callback_t		callback,
					 void*			user_param){
	if (eventtype == ei_ev_keydown) {
		ei_button_t* button = (ei_button_t *) widget;
		button->callback = &callback;

	}

}

void		ei_unbind		(ei_eventtype_t		eventtype,
					 ei_widget_t*		widget,
					 ei_tag_t		tag,
					 ei_callback_t		callback,
					 void*			user_param){

}
