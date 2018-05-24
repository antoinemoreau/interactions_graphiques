#include "ei_application_utils.h"

void            draw_all_widgets        (ei_widget_t*           widget,
                                         ei_surface_t           root_surface,
                                         ei_surface_t           pick_surface,
                                         ei_rect_t*             content_rect,
                                         ei_linked_rect_t**     list_rects) {
        if (widget != NULL) {
                widget->wclass->drawfunc(widget, root_surface, pick_surface, content_rect);
                ei_linked_rect_t* new_rect = calloc(1, sizeof(ei_linked_rect_t));
                new_rect->rect = *(widget->content_rect);
                new_rect->next = *list_rects;
                *list_rects = new_rect;
                draw_all_widgets(widget->children_head, root_surface, pick_surface, widget->content_rect, list_rects);
                draw_all_widgets(widget->next_sibling, root_surface, pick_surface, widget->content_rect, list_rects);

        }
}

ei_widget_t*    mouse_capture           (ei_event_t*            event,
                                         ei_surface_t           pick_surface,
                                         ei_surface_t           root) {
        ei_point_t where_mouse = event->param.mouse.where;
        ei_rect_t rect_pick = hw_surface_get_rect(pick_surface);
        hw_surface_lock(pick_surface);
        uint8_t* buffer_picking = hw_surface_get_buffer(pick_surface);
        hw_surface_unlock(pick_surface);
        buffer_picking += (where_mouse.x + where_mouse.y*rect_pick.size.width)*4;
        ei_color_t mouse_color = {*(buffer_picking+2),*(buffer_picking+1),*(buffer_picking),*(buffer_picking+3)};
        uint32_t mouse_id = ei_map_rgba(pick_surface, &mouse_color);
        return ei_pick_widget(mouse_id, root);
}

void            redraw                  (ei_surface_t           root_surface,
                                         ei_surface_t           pick_surface,
                                         ei_widget_t*           widget,
                                         ei_linked_rect_t**     rect_list) {
        hw_surface_lock(root_surface);
        hw_surface_lock(pick_surface);
        draw_all_widgets(widget, root_surface, pick_surface, widget->content_rect, rect_list);
        hw_surface_unlock(pick_surface);
        hw_surface_unlock(root_surface);
        hw_surface_update_rects(root_surface, *rect_list);
}

void            handle_event            (ei_linked_event_t*     event_list,
                                         ei_event_t*            event,
                                         ei_widget_t*           widget) {
        ei_bool_t no_callback = EI_FALSE;
        ei_linked_event_t* current_event = event_list;



        while(!no_callback && current_event){
                if (current_event->eventtype == event->type) {
                        if(current_event->tag ){
                                if (strcmp(current_event->tag,"all") == 0) {
                                        no_callback = (*(current_event->callback))(widget, event, current_event->user_param);
                                } else if (strcmp(current_event->tag, widget->wclass->name) == 0) {
                                        no_callback = (*(current_event->callback))(widget, event, current_event->user_param);
                                }
                        }else if (current_event->widget == widget){
                                no_callback = (*(current_event->callback))(widget, event, current_event->user_param);
                        }
                }
                current_event = current_event->next;
        }
}

void            release_rect_list       (ei_linked_rect_t**      rect_list) {
        if (*rect_list != NULL) {
                ei_linked_rect_t* del = NULL;
                while(*rect_list != NULL) {
                        del = *rect_list;
                        *rect_list = (*rect_list)->next;
                        free(del);
                }
        }
}