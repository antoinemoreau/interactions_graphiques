#ifndef EI_APP_UTILS_H
#define EI_APP_UTILS_H

#include "ei_widget.h"
#include "hw_interface.h"
#include "ei_event.h"
#include "ei_event_utils.h"
#include <stdlib.h>
#include <string.h>

/**
 * Parcours en largeur récursif affichant tous les widgets
 */
void            draw_all_widgets        (ei_widget_t*           widget,
                                         ei_surface_t           root_surface,
                                         ei_surface_t           pick_surface,
                                         ei_rect_t*             content_rect,
                                         ei_linked_rect_t**     list_rects);

/**
 * Renvoit le widget sous le pointeur de souris.
 */
ei_widget_t*    mouse_capture           (ei_event_t*            event,
                                         ei_surface_t           pick_surface,
                                         ei_surface_t           root);
/**
 * Redessine un widget et tous ses fils.
 */
void            redraw                  (ei_surface_t           root_surface,
                                          ei_surface_t          pick_surface,
                                          ei_widget_t*          widget,
                                          ei_linked_rect_t**    rect_list);

void            handle_event            (ei_linked_event_t*     event_list,
                                           ei_event_t*          event,
                                           ei_widget_t*         widget) ;

void            release_rect_list       (ei_linked_rect_t**      rect_list);

#endif
