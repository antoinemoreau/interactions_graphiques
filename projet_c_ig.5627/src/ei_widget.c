#include "ei_widget.h"
#include "ei_frame.h"
#include "ei_button.h"
#include "ei_geometrymanager.h"
#include <stdlib.h>
#include <stdio.h>

ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
							 ei_widget_t*		parent) {
	ei_widgetclass_t* widgetclass = ei_widgetclass_from_name(class_name);
	//On vérifie si la classe de ce widget existe
	if (widgetclass) {
		ei_widget_t* widget = widgetclass->allocfunc();
		widget->wclass = widgetclass;

		//id et color a modifier pour gestion des evenements
		widget->pick_id = 1;
		widget->pick_color = &ei_default_background_color;

		//affectation du widget parent
		widget->parent = parent;
		if(parent){
			if (parent->children_head == NULL) {
				parent->children_head = widget;
				parent->children_tail = widget;
			} else {
				parent->children_tail->next_sibling = widget;
				parent->children_tail = widget;
			}
		}
		widget->children_head = NULL;
		widget->children_tail = NULL;
		widget->next_sibling = NULL;

		//a modifier
		widget->geom_params = NULL;
		widget->requested_size.width = 0;
		widget->requested_size.height = 0;
		widget->screen_location.top_left.x = 0;
		widget->screen_location.top_left.y = 0;
		widget->screen_location.size.width = 0;
		widget->screen_location.size.height = 0;
		widget->content_rect = &widget->screen_location;

		widgetclass->setdefaultsfunc(widget);
		return widget;
	}
	return NULL;
}



void ei_widget_destroy (ei_widget_t* widget) {

}

ei_widget_t* ei_widget_pick (ei_point_t* where) {
	return NULL;
}

void		ei_frame_configure		(ei_widget_t*		widget,
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
	if (requested_size != NULL){
		widget->requested_size = *requested_size;
	}
	else {
		widget->requested_size.height = 0;
		widget->requested_size.width = 0;
	}
	ei_frame_t* frame = (ei_frame_t*)widget;

	if (color != NULL)
		frame->color = color;
	else if (frame->color == NULL)
		frame->color = &ei_default_background_color;

	if (border_width != NULL)
		frame->border_width = *border_width;
	else if (!frame->border_width)
		frame->border_width = 0;

	if (relief != NULL)
		frame->relief = *relief;
	else if (!frame->relief)
		frame->relief = ei_relief_none;

	if (text != NULL)
		frame->text = text;

	if (text_font != NULL)
		frame->text_font = text_font;
	else if (frame->text_font == NULL)
		frame->text_font = ei_default_font;

	if (text_color != NULL)
		frame->text_color = text_color;
	else if (frame->text_color == NULL)
		frame->text_color = &ei_font_default_color;

	if (text_anchor != NULL)
		frame->text_anchor = *text_anchor;
	else if (!frame->text_anchor)
		frame->text_anchor = ei_anc_center;

	if (img != NULL)
		frame->img = img;

	if (img_rect != NULL)
		frame->img_rect = img_rect;

	if (img_anchor != NULL)
		frame->img_anchor = *img_anchor;
	else if (!frame->img_anchor)
		frame->img_anchor = ei_anc_center;
}

void			ei_button_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 int*			corner_radius,
							 ei_relief_t*		relief,
							 char**			text,
							 ei_font_t*		text_font,
							 ei_color_t*		text_color,
							 ei_anchor_t*		text_anchor,
							 ei_surface_t*		img,
							 ei_rect_t**		img_rect,
							 ei_anchor_t*		img_anchor,
							 ei_callback_t*		callback,
							 void**			user_param) {
	if (requested_size != NULL){
		widget->requested_size = *requested_size;
	}
	else {
		widget->requested_size.height = 0;
		widget->requested_size.width = 0;
	}
	ei_button_t* button = (ei_button_t*)widget;

	if (color != NULL)
		button->color = color;
	else if (button->color == NULL)
		button->color = &ei_default_background_color;

	if (border_width != NULL)
		button->border_width = *border_width;
	else if (!button->border_width)
		button->border_width = 0;

	if (corner_radius != NULL)
		button->corner_radius = *corner_radius;
	else if (!button->corner_radius)
		button->corner_radius = 0;

	if (relief != NULL)
		button->relief = *relief;
	else if (!button->relief)
		button->relief = ei_relief_none;

	if (text != NULL)
		button->text = text;

	if (text_font != NULL)
		button->text_font = text_font;
	else if (button->text_font == NULL)
		button->text_font = ei_default_font;

	if (text_color != NULL)
		button->text_color = text_color;
	else if (button->text_color == NULL)
		button->text_color = &ei_font_default_color;

	if (text_anchor != NULL)
		button->text_anchor = *text_anchor;
	else if (!button->text_anchor)
		button->text_anchor = ei_anc_center;

	if (img != NULL)
		button->img = img;

	if (img_rect != NULL)
		button->img_rect = img_rect;

	if (img_anchor != NULL)
		button->img_anchor = *img_anchor;
	else if (!button->img_anchor)
		button->img_anchor = ei_anc_center;

	if (callback != NULL)
		button->callback = callback;

	if (user_param != NULL)
		button->user_param = user_param;
}

void			ei_toplevel_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 ei_color_t*		color,
							 int*			border_width,
							 char**			title,
							 ei_bool_t*		closable,
							 ei_axis_set_t*		resizable,
						 	 ei_size_t**		min_size) {

}
