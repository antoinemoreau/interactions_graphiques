#include "ei_application_utils.h"

void            draw_all_widgets        (ei_widget_t*   widget,
                                         ei_surface_t   root_surface,
                                         ei_surface_t   pick_surface) {
        if (widget != NULL) {
                printf("affichage d'un wigdet\n");
                widget->wclass->drawfunc(widget, root_surface, pick_surface, widget->content_rect);
                for (ei_widget_t* current = widget->children_head; current != NULL; current = current->next_sibling) {
                        draw_all_widgets(current, root_surface, pick_surface);
                }
        }
}
