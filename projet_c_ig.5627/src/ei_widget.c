#include "ei_widget.h"

ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
							 ei_widget_t*		parent) {
	ei_widgetclass_t* widgetclass = ei_widgetclass_from_name(class_name);
	//On vérifie si la classe de ce widget existe
	if (widgetclass) {
		ei_widget_t* widget = *(widgetclass->allocfunc);
	}
}

void			ei_widget_destroy		(ei_widget_t*		widget){

}

ei_widget_t*		ei_widget_pick			(ei_point_t*		where){

}



void			ei_frame_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 ei_relief_t*		relief,
							 char**			text,
							 ei_font_t*		text_font,
							 ei_color_t*		text_color,
							 ei_anchor_t*		text_anchor,
							 ei_surface_t*		img,
							 ei_rect_t**		img_rect,
							 ei_anchor_t*		img_anchor) {

}
