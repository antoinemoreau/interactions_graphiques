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
 * @brief               Draw all the widgets in a sub-tree which root is a given widget.
 * 
 * @param widget                The root of the sub-tree.
 * @param root_surface          The root_surface.
 * @param pick_surface          The picking surface.
 * @param content_rect          The content rectangle.
 */
void draw_all_widgets(ei_widget_t *widget,
                      ei_surface_t root_surface,
                      ei_surface_t pick_surface,
                      ei_rect_t *content_rect);

/**
 * @brief               Redraw a widget and all his children.
 * 
 * @param root_surface          The root surface.
 * @param pick_surface          The picking surface.
 * @param widget                The widget to draw.
 * @param rect_list             The rectangle list to update on screen.
 */
void redraw(ei_surface_t root_surface,
            ei_surface_t pick_surface,
            ei_widget_t *widget,
            ei_linked_rect_t *rect_list);

/**
 * @brief               Handle the given event.
 * 
 * @param event_list            The list of bind events.
 * @param event                 The given event.
 * @param widget                The widget under the mouse pointer.
 */
void handle_event(ei_linked_event_t *event_list,
                  ei_event_t *event,
                  ei_widget_t *widget);

/**
 * @brief               Get the rectangle list to update.
 * 
 * @return ei_linked_rect_t**   The rectangle list
 */
ei_linked_rect_t **get_rect_list();

/**
 * @brief               Add a rectangle to a rectangle list.
 * 
 * @param rect_list             The list.
 * @param rect                  The rectangle to add.
 */
void rect_list_add(ei_linked_rect_t **rect_list,
                   ei_rect_t rect);

/**
 * @brief               Free the rectangle list.
 * 
 * @param rect_list             The rectangle list to free.
 */
void release_rect_list(ei_linked_rect_t **rect_list);

/**
 * @brief               Get the picking surface.
 * 
 * @return ei_surface_t         The picking surface.
 */
ei_surface_t ei_app_pick_surface();

#endif
