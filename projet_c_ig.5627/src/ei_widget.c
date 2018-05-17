#include "ei_widget.h"
#include <stdlib.h>

ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
							 ei_widget_t*		parent) {
	ei_widgetclass_t* widgetclass = ei_widgetclass_from_name(class_name);
	//On vérifie si la classe de ce widget existe
	if (widgetclass) {
		ei_widget_t* widget = widgetclass->allocfunc();
		widget->wclass = widgetclass;

		//id et color a modifier pour gestion des evenements
		widget->pick_id = 1;
		*(widget->pick_color) = ei_default_background_color;

		//affectation du widget parent
		widget->parent = parent;
		widget->children_head = NULL;
		widget->children_tail = NULL;

		//a modifier lorsqu'on aura plusieurs widgets
		widget->next_sibling = NULL;

		//a modifier
		widget->geom_params = NULL;

		widgetclass->setdefaultsfunc(widget);
		return widget;
	}
	return NULL;
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
