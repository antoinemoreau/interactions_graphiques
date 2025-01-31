#include "ei_types.h"
#include "ei_widget.h"
#include "ei_event.h"
#include "ei_button.h"
#include "ei_event_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>

void ei_bind(ei_eventtype_t eventtype,
	     ei_widget_t *widget,
	     ei_tag_t tag,
	     ei_callback_t callback,
	     void *user_param)
{
	ei_linked_event_t *list_of_events = get_list_events();
	ei_linked_event_t *event = calloc(1, sizeof(ei_linked_event_t));
	event->eventtype = eventtype;
	event->widget = widget;
	event->tag = tag;
	event->callback = callback;
	event->user_param = user_param;
	event->next = list_of_events;
	list_of_events = event;
	set_list_events(list_of_events);
}

void ei_unbind(ei_eventtype_t eventtype,
	       ei_widget_t *widget,
	       ei_tag_t tag,
	       ei_callback_t callback,
	       void *user_param)
{
	ei_linked_event_t *prec = get_list_events();
	ei_linked_event_t *del = NULL;
	// If the events list is empty
	if (prec == NULL)
	{
		fprintf(stderr, "Can't unbind event. Event list is empty.\n");
		exit(1);
	}
	else
	{
		// Looping over events to find the event to delete
		while (prec->next != NULL)
		{
			if (prec->next->eventtype == eventtype && prec->next->widget == widget && strcmp(prec->next->tag, tag) == 0 && prec->next->user_param == user_param)
			{
				del = prec->next;
				break;
			}
			else
			{
				prec = prec->next;
			}
		}

		// Deleting event if it exists
		if (del == NULL)
		{
			fprintf(stderr, "Event does not exist.\n");
			exit(1);
		}
		else
		{
			prec->next = del->next;
			free(del->user_param);
			free(del);
		}
	}
}
