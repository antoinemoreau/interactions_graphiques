#ifndef EI_FRAME_H
#define EI_FRAME_H
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"

/**
 * @brief 	A structure which describes a frame widget.
 * 
 */
typedef struct ei_frame_t {
	ei_widget_t		widget;
	ei_color_t		color;
	int			border_width;
	ei_relief_t		relief;
	char* 			text;
	ei_font_t		text_font;
	ei_color_t		text_color;
	ei_anchor_t		text_anchor;
	ei_surface_t		img;
	ei_rect_t*		img_rect;
	ei_anchor_t		img_anchor;
} ei_frame_t;

#endif
