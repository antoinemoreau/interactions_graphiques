#include "ei_event.h"


typedef struct ei_linked_event_t {
	ei_widget_t*                    widget;
	ei_eventtype_t			eventtype;
        ei_tag_t                        tag;
        ei_callback_t                   callback;
	void*				user_param;
	struct ei_linked_event_t*       next;
} ei_linked_event_t;


ei_linked_event_t* list_events();

void ei_init_list_events();

ei_bool_t pressbutton_animation(ei_widget_t*, struct ei_event_t* event, void* user_param);
