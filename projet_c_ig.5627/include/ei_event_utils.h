#include "ei_event.h"

typedef struct ei_linked_event_t {
	ei_widget_t*                    widget;
	ei_eventtype_t			eventtype;
        ei_tag_t                        tag;
        ei_callback_t                   callback;
	void*				user_param;
	struct ei_linked_event_t*       next;
} ei_linked_event_t;


ei_linked_event_t* get_list_events();

void set_list_events (ei_linked_event_t* list);

void ei_init_list_events();

ei_linked_event_t* find_event(ei_widget_t* widget, ei_eventtype_t eventtype, ei_tag_t tag, ei_callback_t callback);

ei_bool_t pressbutton_animation(ei_widget_t*, struct ei_event_t* event, void* user_param);

ei_bool_t unpressbutton_animation(ei_widget_t*, struct ei_event_t* event, void* user_param);


ei_widget_t* ei_pick_widget(uint32_t pick_id, ei_widget_t* widget);
