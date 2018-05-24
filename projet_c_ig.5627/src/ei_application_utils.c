#include "ei_application_utils.h"

void            draw_all_widgets        (ei_widget_t*           widget,
                                         ei_surface_t           root_surface,
                                         ei_surface_t           pick_surface,
                                         ei_rect_t*             content_rect,
                                         ei_linked_rect_t*      list_rects) {
        if (widget != NULL) {
                widget->wclass->drawfunc(widget, root_surface, pick_surface, content_rect);
                ei_linked_rect_t* new_rect = calloc(1, sizeof(ei_linked_rect_t));
                new_rect->rect = *(widget->content_rect);
                new_rect->next = list_rects;
                list_rects = new_rect;
                for (ei_widget_t* current = widget->children_head; current != NULL; current = current->next_sibling) {
                        draw_all_widgets(current, root_surface, pick_surface, widget->content_rect, list_rects);
                }
        }
}
