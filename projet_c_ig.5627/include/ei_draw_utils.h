#include "ei_draw.h"
#include "ei_widget.h"
#include <stdio.h>

/*
A new structure to help us getting the head and the tail point of a linked list of points
(Namely used when calling \ref rounded_frame in order to gather the arcs)
*/

typedef struct {
	ei_linked_point_t*			head_point;
	ei_linked_point_t*			tail_point;
} ei_extreme_linked_points_t;

/**
 * \brief	Compute a new color according to the initiale one and a variation.
 *		Namely used to draw buttons' borders.
 *
 * @param	initiale		The color initially given.
 *
 * @param	variation		The coefficient used to compute the R, G and B component of the new color
 *					given initial values.
 *
 * @param	res			The new color (usually for the border).
 */

void ei_compute_color(ei_color_t initiale, ei_color_t* res, float variation);

/**
 * \brief	Computes the location of the point linked to the anchor (ex: northeast will give the position of
 *		the top right point, southwest of the bot left point)
 *
 *
 * @param	anchor		The anchor which is related to the point we will compute the location.
 *
 * @param	rectangle			The rectangle that is linked to its parent with the @param anchor.
 *
 * @param	texte				The text inside the rectangle.
 *
 * @param	anchor_position			The position of the anchor.
 */

void ei_anchor_spot(ei_anchor_t anchor, ei_size_t* texte, ei_rect_t* rectangle, ei_point_t* anchor_position);

/**
 * \brief	Computes the intersection between two rectangles
 *
 * @param	first_rect		The first rectangle in the area.
 *
 * @param	sec_rect		The seconde rectangle in the area.
 *
 * @param	dest			The new rectangle (intersection of the fist two).
 */

void ei_intersection_rectangle(ei_rect_t* first_rect, ei_rect_t* sec_rect, ei_rect_t* dest);

/**
 * \brief	This function will compute a given number of points to draw an arc with \ref rounded_frame.
 *
 * @param	center			The center of the arc.
 *
 * @param	radius			The radius of the arc.
 *
 * @param	starting_angle		The angle to start the arc.
	*
 * @ param	ending_angle		The angle to end the arc.
 *
 * @param	nb_points		The number of points used to draw the arc (i.e. the subdivision).
 *
 * @return	The head and the tail of the linked points.
 */

ei_extreme_linked_points_t* arc(ei_point_t center, int radius, float starting_angle, float ending_angle, int nb_points);

void ei_free_polygon(ei_linked_point_t** poly);
