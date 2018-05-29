#ifndef EI_BUTTONCLASS_H
#define EI_BUTTONCLASS_H

#include <string.h>
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_button.h"

void* ei_button_allocfunc ();

void ei_button_releasefunc (struct ei_widget_t* button);

void ei_button_drawfunc (struct ei_widget_t* button, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper);

void ei_button_setdefaultsfunc (struct ei_widget_t* button);

void ei_button_geomnotifyfunc (struct ei_widget_t* button, ei_rect_t rect);

/**
 * \brief	Linkes points to draw the polygone built with the latter (according to the part chosen).
 *              Namely used to draw button' corners.
 *
* @param	rectangle		The rectangle we are rounding the corners.
 *
 * @param	radius		        The radius of the corners.
 *
 * @param	nb_points		The number of points used to build the arcs.
 *
 * @param	part			The part we want to round according to the specification
 *                                      (0 : rounds the bot part
 *                                       1 : rounds the top part
 *                                       2 : rounds every corners' rectangle)
 *
 * @return      
 */

ei_linked_point_t* rounded_frame(ei_rect_t rectangle, int radius, int nb_points, int part);


#endif
