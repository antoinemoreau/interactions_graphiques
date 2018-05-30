#ifndef EI_APP_UTILS_H
#define EI_APP_UTILS_H

#include "ei_widget.h"
#include "hw_interface.h"
#include "ei_event.h"
#include "ei_event_utils.h"
#include <stdlib.h>
#include <string.h>

ei_surface_t pick_surface;

/**
 * Parcours en largeur récursif affichant tous les widgets
 */
void draw_all_widgets(ei_widget_t *widget,
                      ei_surface_t root_surface,
                      ei_surface_t pick_surface,
                      ei_rect_t *content_rect);

/**
 * Redessine un widget et tous ses fils.
 */
void redraw(ei_surface_t root_surface,
            ei_surface_t pick_surface,
            ei_widget_t *widget,
            ei_linked_rect_t *rect_list);

void handle_event(ei_linked_event_t *event_list,
                  ei_event_t *event,
                  ei_widget_t *widget);

ei_linked_rect_t **get_rect_list();

void rect_list_add(ei_linked_rect_t **rect_list,
                   ei_rect_t rect);

void release_rect_list(ei_linked_rect_t **rect_list);

ei_surface_t ei_app_pick_surface();

#endif
