#ifndef EI_EVENT_UTILS_H
#define EI_EVENT_UTILS_H
#include "ei_event.h"

/**
 * @brief 	A structure which describes a list of events.
 * 
 */
typedef struct ei_linked_event_t
{
	ei_widget_t *widget;
	ei_eventtype_t eventtype;
	ei_tag_t tag;
	ei_callback_t callback;
	void *user_param;
	struct ei_linked_event_t *next;
} ei_linked_event_t;

extern ei_bool_t drawing;
extern ei_bool_t destroy;

ei_linked_event_t *get_list_events();

/**
 * @brief 	Set the list events object.
 * 
 * @param	list		The list to modify.
 */
void set_list_events(ei_linked_event_t *list);

/**
 * @brief  Free the list events object.
 */
void free_list_events();

/**
 * @brief  Create the list events objects.
 */
void ei_init_list_events();

/**
 * @brief	Find the event matching to specific prameters.
 * 
 * @param widget 		The widget.
 * @param eventtype 		The type of event.
 * @param tag 			The tag.
 * @param callback 		The callback function.
 * @return ei_linked_event_t*	A pointer on the event if it exists, otherwise NULL.
 */
ei_linked_event_t *find_event(ei_widget_t *widget,
			      ei_eventtype_t eventtype,
			      ei_tag_t tag,
			      ei_callback_t callback);

/**
 * @brief	Find the widget with its pick_id.
 * 
 * @param pick_id 		The pick_id.
 * @param widget 		The widget where to start recursion.
 * @return ei_widget_t* 	The widget if it exists, otherwise NULL.
 */
ei_widget_t *ei_find_widget(uint32_t pick_id,
			    ei_widget_t *widget);

/**
 * @brief 	Animate a button when it is pressed.
 */
ei_bool_t pressbutton_animation(ei_widget_t *widget,
				struct ei_event_t *event,
				void *user_param);

/**
 * @brief 	Animate a button when it is unpressed.
 */
ei_bool_t unpressbutton_animation(ei_widget_t *widget,
				  struct ei_event_t *event,
				  void *user_param);

/**
 * @brief 	Animate a button when the mouse pointer goes out of the button.
 */
ei_bool_t getoutofbutton_animation(ei_widget_t *widget,
				   struct ei_event_t *event,
				   void *user_param);

/**
 * @brief 	Handle the clic on the title bar of a toplevel widget in order to move it.
 */
ei_bool_t click_toplevel_header(ei_widget_t *widget,
				struct ei_event_t *event,
				void *user_param);

/**
 * @brief 	Move a toplevel widget.
 */
ei_bool_t move_toplevel(ei_widget_t *widget,
			struct ei_event_t *event,
			void *user_param);

/**
 * @brief 	Stop moving the toplevel when the mouse button is released.	
 */
ei_bool_t unclick_toplevel(ei_widget_t *widget,
			   struct ei_event_t *event,
			   void *user_param);

/**
 * @brief 	Handle the clic on the resizing zone of a toplevel widget in order to resize it.
 */
ei_bool_t click_resize_toplevel(ei_widget_t *widget, struct ei_event_t *event, void *user_param);

/**
 * @brief 	Resize the toplevel widget.
 */
ei_bool_t resizing_toplevel(ei_widget_t *widget, struct ei_event_t *event, void *user_param);

/**
 * @brief 	Stop resizing the toplevel when the mouse button is released.
 */
ei_bool_t stop_resize(ei_widget_t *widget, struct ei_event_t *event, void *user_param);

/**
 * @brief 	Move a widget to foreground when clicking on it.
 */
ei_bool_t move_foreground(ei_widget_t *widget, struct ei_event_t *event, void *user_param);

#endif
