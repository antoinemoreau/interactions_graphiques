#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ei_event_utils.h"
#include "ei_toplevel.h"
#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "ei_application_utils.h"
#include "ei_draw_utils.h"
#include "ei_placer.h"
#include "ei_utils.h"

ei_bool_t drawing = EI_FALSE;
ei_bool_t destroy = EI_FALSE;
ei_linked_event_t *listed_events;

ei_linked_event_t *get_list_events()
{
        return listed_events;
}

// Button animation
ei_button_t *sunken_button;

// Toplevel move
ei_toplevel_t *moving_toplevel;
ei_point_t mouse_pos;

// toplevel resize
ei_toplevel_t *resized_toplevel;
ei_point_t mouse_resize;

void set_list_events(ei_linked_event_t *list)
{
        listed_events = list;
}

void free_list_events()
{
        ei_linked_event_t *del = listed_events;
        while (listed_events != NULL)
        {
                listed_events = listed_events->next;
                free(del);
                del = listed_events;
        }
}

void ei_init_list_events()
{

        // Button animations
        ei_bind(ei_ev_mouse_move, NULL, "all", (ei_callback_t)&getoutofbutton_animation, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "button", (ei_callback_t)&pressbutton_animation, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "button", (ei_callback_t)&unpressbutton_animation, NULL);

        // Toplevel move
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", (ei_callback_t)&click_toplevel_header, NULL);
        ei_bind(ei_ev_mouse_move, NULL, "all", (ei_callback_t)&move_toplevel, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "toplevel", (ei_callback_t)&unclick_toplevel, NULL);

        // Toplevel resize
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", (ei_callback_t)&click_resize_toplevel, NULL);
        ei_bind(ei_ev_mouse_move, NULL, "all", (ei_callback_t)&resizing_toplevel, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "toplevel", (ei_callback_t)&stop_resize, NULL);

        // Move widget to foreground
        ei_bind(ei_ev_mouse_buttondown, NULL, "all", (ei_callback_t)&move_foreground, NULL);
}

ei_linked_event_t *find_event(ei_widget_t *widget, ei_eventtype_t eventtype, ei_tag_t tag, ei_callback_t callback)
{
        ei_linked_event_t *current = get_list_events();
        while (current)
        {
                if (current->widget == widget && current->eventtype == eventtype && current->tag == tag && current->callback == callback)
                {
                        return current;
                }
                else
                {
                        current = current->next;
                }
        }
        fprintf(stderr, "This event is not binded.\n");
        exit(1);
}

ei_widget_t *ei_find_widget(uint32_t pick_id, ei_widget_t *widget)
{
        if (pick_id == widget->pick_id)
        {
                return widget;
        }
        ei_widget_t *child = widget->children_head;
        while (child)
        {
                ei_widget_t *pick = ei_find_widget(pick_id, child);
                if (pick)
                {
                        return pick;
                }
                child = child->next_sibling;
        }
        return NULL;
}

// Button callback functions
ei_bool_t unpressbutton_animation(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        if (sunken_button && event->param.mouse.button_number == 1)
        {
                sunken_button->relief = ei_relief_raised;
                sunken_button = NULL;
        }
        return EI_FALSE;
}

ei_bool_t pressbutton_animation(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        if (event->param.mouse.button_number == 1)
        {
                sunken_button = (ei_button_t *)widget;
                sunken_button->relief = ei_relief_sunken;
        }
        return EI_FALSE;
}

ei_bool_t getoutofbutton_animation(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        if (sunken_button && widget->pick_id != sunken_button->widget.pick_id)
        {
                sunken_button->relief = ei_relief_raised;
                sunken_button = NULL;
        }
        return EI_FALSE;
}

// Toplevel callback functions
ei_bool_t click_toplevel_header(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        if (event->param.mouse.where.y < widget->content_rect->top_left.y && event->param.mouse.button_number == 1)
        {
                moving_toplevel = (ei_toplevel_t *)widget;
                mouse_pos = event->param.mouse.where;
        }
        return EI_FALSE;
}

ei_bool_t move_toplevel(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        // If a toplevel title bar is currently clicked
        if (moving_toplevel)
        {
                ei_widget_t *moving_widget = (ei_widget_t *)moving_toplevel;
                ei_linked_rect_t **rect_list = get_rect_list();
                ei_rect_t intersection1;
                ei_intersection_rectangle(moving_widget->parent->content_rect, &moving_widget->screen_location, &intersection1);
                rect_list_add(rect_list, intersection1);

                // Updating the placer parameters
                ei_placer_t *placer = (ei_placer_t *)moving_widget->geom_params;
                ei_point_t shift_position = ei_point_sub(event->param.mouse.where, mouse_pos);
                int new_x = placer->x + shift_position.x;
                int new_y = placer->y + shift_position.y;
                moving_widget->screen_location.top_left = ei_point_add(moving_widget->screen_location.top_left, shift_position);
                ei_place(moving_widget, &placer->anchor, &new_x, &new_y, &placer->width, &placer->height, &placer->rel_x, &placer->rel_y, &placer->rel_width, &placer->rel_height);
                mouse_pos = event->param.mouse.where;

                // Updating the geometry of the toplevel children
                ei_widget_t *current = moving_widget->children_head;
                while (current)
                {
                        current->geom_params->manager->runfunc(current);
                        ei_widget_t *current_bro = current->next_sibling;
                        while (current_bro)
                        {
                                current_bro->geom_params->manager->runfunc(current_bro);
                                current_bro = current_bro->next_sibling;
                        }
                        current = current->children_head;
                }

                ei_rect_t intersection2;
                ei_intersection_rectangle(moving_widget->parent->content_rect, &moving_widget->screen_location, &intersection2);
                rect_list_add(rect_list, intersection2);
        }
        return EI_FALSE;
}

ei_bool_t unclick_toplevel(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        if (moving_toplevel && event->param.mouse.button_number == 1)
        {
                moving_toplevel = NULL;
        }
        return EI_FALSE;
}

ei_bool_t click_resize_toplevel(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        // Checking if the mouse is in the right location on the toplevel
        if ((widget->screen_location.top_left.y + widget->screen_location.size.height -
             4 * ((ei_toplevel_t *)widget)->border_width) < event->param.mouse.where.y &&
            event->param.mouse.where.y < (widget->screen_location.top_left.y +
                                          widget->screen_location.size.height) &&
            (widget->screen_location.top_left.x + widget->screen_location.size.width -
                4 * ((ei_toplevel_t *)widget)->border_width) < event->param.mouse.where.x &&
                event->param.mouse.where.x < (widget->screen_location.top_left.x + widget->screen_location.size.width)
                && event->param.mouse.button_number == 1)
        {
                mouse_resize = event->param.mouse.where;
                resized_toplevel = (ei_toplevel_t *)widget;
        }
        return EI_FALSE;
}

ei_bool_t resizing_toplevel(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        if (resized_toplevel)
        {
                ei_linked_rect_t **rect_list = get_rect_list();
                ei_widget_t *resized_widget = (ei_widget_t *)resized_toplevel;
                ei_rect_t intersection1;
                ei_intersection_rectangle(resized_widget->parent->content_rect, &resized_widget->screen_location, &intersection1);
                rect_list_add(rect_list, intersection1);
                // Computation the size of the toplevel according to the new and old mouse position
                int width_loss = mouse_resize.x - event->param.mouse.where.x;
                int height_loss = mouse_resize.y - event->param.mouse.where.y;
                int diff_x = resized_widget->screen_location.size.width - width_loss;
                int diff_y = resized_widget->screen_location.size.height - height_loss;
                // Checking if the new size if bigger than the minimum size specified in the documentation
                if (diff_x > resized_toplevel->min_size->width && diff_y > resized_toplevel->min_size->height)
                {
                        // Resizing according differents axis
                        if (ei_axis_none)
                        {
                                resized_toplevel = NULL;
                                return EI_FALSE;
                        }
                        // Resizing on the y-axis only
                        else if (ei_axis_y)
                        {
                                diff_x = 0;
                        }
                        // Resizing on the x-axis only
                        else if (ei_axis_x)
                        {
                                diff_y = 0;
                        }
                        resized_widget->screen_location.size = ei_size(diff_x, diff_y);
                        mouse_resize = event->param.mouse.where;
                        ei_placer_t *placer = (ei_placer_t *)resized_widget->geom_params;
                        int width_abs = placer->width - width_loss;
                        int height_abs = placer->height - height_loss;
                        ei_place(resized_widget, &placer->anchor, &placer->x, &placer->y, &width_abs, &height_abs, &placer->rel_x, &placer->rel_y, &placer->rel_width, &placer->rel_height);
                        ei_rect_t intersection2;
                        ei_intersection_rectangle(resized_widget->parent->content_rect, &resized_widget->screen_location, &intersection2);
                        rect_list_add(rect_list, intersection2);
                        // Replacing the children and their next_sibling according to the parent new size
                        ei_widget_t *current = resized_widget->children_head;
                        while (current)
                        {
                                current->geom_params->manager->runfunc(current);
                                ei_widget_t *current_bro = current->next_sibling;
                                while (current_bro)
                                {
                                        current_bro->geom_params->manager->runfunc(current_bro);
                                        current_bro = current_bro->next_sibling;
                                }
                                current = current->children_head;
                        }
                }
        }
        return EI_FALSE;
}

ei_bool_t stop_resize(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        if (resized_toplevel)
        {
                resized_toplevel = NULL;
        }
        return EI_FALSE;
}

/**
 * @brief       Move a widget to the tail of its siblings list.
 * 
 * @param widget        The widget to move.
 * 
 * @param previous      The previous widget in the list.
 *                      May be NULL is widget is the head of the list
 */
static void move_elt_to_end(ei_widget_t *widget, ei_widget_t *previous)
{
        if (widget && widget->parent)
        {
                previous = (widget->parent->children_head != widget) ? widget->parent->children_head : NULL;
                if (previous)
                {
                        while (previous->next_sibling != widget)
                        {
                                previous = previous->next_sibling;
                        }
                        previous->next_sibling = widget->next_sibling;
                }
                else
                {
                        widget->parent->children_head = widget->next_sibling;
                }
                widget->next_sibling = NULL;
                widget->parent->children_tail->next_sibling = widget;
                widget->parent->children_tail = widget;
        }
}

ei_bool_t move_foreground(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
        // Moving te widget and all his ancestors to the tail of their respective siblings lists.

        ei_widget_t *previous = NULL;
        ei_widget_t *second_widget = NULL;
        ei_bool_t is_toplevel = strcmp(widget->wclass->name, "toplevel") == 0;

        if (!widget->parent)
                return EI_FALSE;

        // If the widget is already the tail, jump to parent
        if (!widget->next_sibling || (is_toplevel && !widget->next_sibling->next_sibling))
                return move_foreground(widget->parent, event, user_param);

        // Moving the widget to tail (May have to move 2 widgets if widget is a toplevel)
        if (is_toplevel)
        {
                second_widget = widget->next_sibling;
        }

        move_elt_to_end(widget, previous);
        move_elt_to_end(second_widget, previous);

        return move_foreground(widget->parent, event, user_param);
}
