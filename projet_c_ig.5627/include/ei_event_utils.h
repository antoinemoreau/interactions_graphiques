#include "ei_event.h"


typedef struct ei_linked_event_t {
	ei_widget_t*                   widget;
        ei_tag_t                       tag;
        ei_callback_t                  callback;
	struct ei_linked_event_t*      next;
} ei_linked_event_t;
