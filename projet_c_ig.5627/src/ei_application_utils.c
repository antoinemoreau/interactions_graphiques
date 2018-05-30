#include "ei_application_utils.h"
#include "ei_toplevel.h"

ei_linked_rect_t *rect_list = NULL;
ei_rect_t screen_loc_before_event;

void draw_all_widgets(ei_widget_t *widget,
                      ei_surface_t root_surface,
                      ei_surface_t pick_surface,
                      ei_rect_t *content_rect)
{
        if (widget != NULL)
        {
                widget->wclass->drawfunc(widget, root_surface, pick_surface, content_rect);
                draw_all_widgets(widget->children_head, root_surface, pick_surface, widget->content_rect);
                if (widget->next_sibling != NULL)
                {
                        draw_all_widgets(widget->next_sibling, root_surface, pick_surface, widget->parent->content_rect);
                }
        }
}

void redraw(ei_surface_t root_surface,
            ei_surface_t pick_surface,
            ei_widget_t *widget,
            ei_linked_rect_t *rect_list)
{
        if (widget != NULL)
        {
                hw_surface_lock(root_surface);
                hw_surface_lock(pick_surface);

                // If the screen_location has been modifed, we refresh the parent rectangle
                if (widget->parent && (screen_loc_before_event.top_left.x != widget->screen_location.top_left.x || \
                        screen_loc_before_event.top_left.y != widget->screen_location.top_left.y || \
                        screen_loc_before_event.size.width != widget->screen_location.size.width || \
                        screen_loc_before_event.size.height != widget->screen_location.size.height))
                {

                        widget = widget->parent;
                        rect_list_add(&rect_list, widget->screen_location);
                }

                // Redrawing the sub-tree which root is the given widget
                draw_all_widgets(widget, root_surface, pick_surface, &(widget->screen_location));
                hw_surface_unlock(pick_surface);
                hw_surface_unlock(root_surface);
                hw_surface_update_rects(root_surface, rect_list);
        }
}

void handle_event(ei_linked_event_t *event_list,
                  ei_event_t *event,
                  ei_widget_t *widget)
{
        ei_bool_t no_callback = EI_FALSE;
        ei_linked_event_t *current_event = event_list;

        if (widget)
                screen_loc_before_event = widget->screen_location;

        // Loop over binded events. Stops when there is no more callback to call.
        while (!no_callback && current_event)
        {
                if (current_event->eventtype == event->type)
                {
                        if (current_event->tag)
                        {
                                if (strcmp(current_event->tag, "all") == 0)
                                {
                                        no_callback = (*(current_event->callback))(widget, event, current_event->user_param);
                                        drawing = EI_TRUE;
                                }
                                else if (strcmp(current_event->tag, widget->wclass->name) == 0)
                                {
                                        no_callback = (*(current_event->callback))(widget, event, current_event->user_param);
                                        drawing = EI_TRUE;
                                }
                        }
                        else if (current_event->widget == widget)
                        {
                                no_callback = (*(current_event->callback))(widget, event, current_event->user_param);
                                drawing = EI_TRUE;
                        }
                }
                current_event = current_event->next;
        }
}

ei_linked_rect_t **get_rect_list()
{
        return &rect_list;
}

void rect_list_add(ei_linked_rect_t **rect_list,
                   ei_rect_t rect)
{
        ei_linked_rect_t *new_rect = calloc(1, sizeof(ei_linked_rect_t));
        new_rect->rect = rect;
        new_rect->next = *rect_list;
        *rect_list = new_rect;
}

void release_rect_list(ei_linked_rect_t **rect_list)
{
        if (*rect_list != NULL)
        {
                ei_linked_rect_t *del = NULL;
                while (*rect_list != NULL)
                {
                        del = *rect_list;
                        *rect_list = (*rect_list)->next;
                        free(del);
                }
        }
}

ei_surface_t ei_app_pick_surface()
{
        return pick_surface;
}
