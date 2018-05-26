#include "ei_widget.h"
#include "ei_frame.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_geometrymanager.h"
#include "ei_application_utils.h"
#include "ei_application.h"
#include "ei_draw.h"
#include "ei_event.h"
#include <stdlib.h>
#include <stdio.h>

static ei_color_t pick_color = {0x00, 0x00, 0x00, 0xff};
//static uint32_t pick_id = 0;

static ei_color_t* inc_pick_color () {
	// if (pick_color.alpha < 0xff){
	// 	pick_color.alpha++;
	// } else {
	if (pick_color.blue < 0xff) {
		pick_color.blue++;
	} else {
		pick_color.blue = 0x00;
		if (pick_color.green < 0xff) {
			pick_color.green++;
		} else {
			pick_color.blue = 0x00;
			if (pick_color.red < 0xff) {
				pick_color.red++;
			} else {
				fprintf(stderr, "Seriously, 16 777 216 widgets ?");
				exit(1);
			}
		}
	}
	ei_color_t* color = calloc(1, sizeof(ei_color_t));
	color->red = pick_color.red;
	color->green = pick_color.green;
	color->blue = pick_color.blue;
	color->alpha = pick_color.alpha;
	return color;
}

ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
							 ei_widget_t*		parent) {
	ei_widgetclass_t* widgetclass = ei_widgetclass_from_name(class_name);
	//On vérifie si la classe de ce widget existe
	if (widgetclass) {
		ei_widget_t* widget = widgetclass->allocfunc();
		widget->wclass = widgetclass;

		//id incremente a chaque instanciation de widget
		widget->pick_color = inc_pick_color();
		widget->pick_id = ei_map_rgba(ei_app_pick_surface(), widget->pick_color);


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

		widget->geom_params = NULL;
		widget->requested_size.width = 0;
		widget->requested_size.height = 0;
		widget->screen_location.top_left.x = 0;
		widget->screen_location.top_left.y = 0;
		widget->screen_location.size.width = 0;
		widget->screen_location.size.height = 0;
		widget->content_rect = malloc(sizeof(ei_rect_t));
		widget->content_rect->top_left.x = 0;
		widget->content_rect->top_left.y = 0;
		widget->content_rect->size.width = 0;
		widget->content_rect->size.height = 0;
		widgetclass->setdefaultsfunc(widget);
		return widget;
	}
	return NULL;
}



void ei_widget_destroy (ei_widget_t* widget) {

}

ei_widget_t* ei_widget_pick (ei_point_t* where) {
	ei_surface_t surface = ei_app_pick_surface();
	ei_rect_t rect_picking = hw_surface_get_rect(surface);
	hw_surface_lock(surface);
	uint8_t* buffer_picking = hw_surface_get_buffer(surface);
	buffer_picking += ((where->x) + (where->y)*rect_picking.size.width)*4;
	int pos_r;
	int pos_g;
	int pos_b;
	int pos_a;
	hw_surface_get_channel_indices(surface, &pos_r, &pos_g, &pos_b, &pos_a);
	ei_color_t mouse_color = {buffer_picking[pos_r], buffer_picking[pos_g],buffer_picking[pos_b],buffer_picking[pos_a]};
	uint32_t mouse_id = ei_map_rgba(surface, &mouse_color);
	ei_widget_t *root_widget = ei_app_root_widget();
	return ei_find_widget(mouse_id, root_widget);
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
	else{
		widget->requested_size = widget->content_rect->size;
	}
	ei_frame_t* frame = (ei_frame_t*)widget;

	if (color != NULL)
		frame->color = *color;

	if (border_width != NULL)
		frame->border_width = *border_width;
	else if (!frame->border_width)
		frame->border_width = 0;

	if (relief != NULL)
		frame->relief = *relief;
	else if (!frame->relief)
		frame->relief = ei_relief_none;

	if (text != NULL) {
		if (frame->text == NULL) {
			frame->text = malloc(strlen(*text)+1);
		} else {
			frame->text = realloc(frame->text, strlen(*text)+1);
		}
		strcpy(frame->text, *text);
	}

	if (text_font != NULL)
		frame->text_font = *text_font;
	else if (!frame->text_font)
		frame->text_font = ei_default_font;

	if (text_color != NULL)
		frame->text_color = *text_color;

	if (text_anchor != NULL)
		frame->text_anchor = *text_anchor;
	else if (!frame->text_anchor)
		frame->text_anchor = ei_anc_center;

	if (img != NULL)
		frame->img = *img;

	if (img_rect != NULL)
		frame->img_rect = *img_rect;

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
	} else {
		widget->requested_size.width = 20;
		widget->requested_size.height = 20;
	}

	ei_button_t* button = (ei_button_t*)widget;

	if (color != NULL)
		button->color = *color;

	if (border_width != NULL)
		button->border_width = *border_width;
	else if (!button->border_width)
		button->border_width = 0;

	if (corner_radius != NULL)
		button->corner_radius = *corner_radius;
	else if (!button->corner_radius)
		button->corner_radius = 0;

	if (relief == NULL || *relief == ei_relief_none)
		button->relief = ei_relief_raised;
	else
		button->relief = *relief;

	if (text != NULL) {
		if (button->text == NULL) {
			button->text = malloc(strlen(*text)+1);
		} else {
			button->text = realloc(button->text, strlen(*text)+1);
		}
		strcpy(button->text, *text);
	}

	if (text_font != NULL)
		button->text_font = *text_font;
	else if (!button->text_font)
		button->text_font = ei_default_font;

	if (text_color != NULL)
		button->text_color = *text_color;

	if (text_anchor != NULL)
		button->text_anchor = *text_anchor;
	else if (!button->text_anchor)
		button->text_anchor = ei_anc_center;

	if (img != NULL)
		button->img = *img;

	if (img_rect != NULL)
		button->img_rect = *img_rect;

	if (img_anchor != NULL)
		button->img_anchor = *img_anchor;
	else if (!button->img_anchor)
		button->img_anchor = ei_anc_center;

	if (callback != NULL) {
		button->callback = *callback;
		if (user_param != NULL)
			button->user_param = *user_param;
		ei_bind(ei_ev_mouse_buttondown, (ei_widget_t*)button, NULL, *callback, button->user_param);
	}
}

void			ei_toplevel_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 ei_color_t*		color,
							 int*			border_width,
							 char**			title,
							 ei_bool_t*		closable,
							 ei_axis_set_t*		resizable,
						 	 ei_size_t**		min_size) {

	 if (requested_size != NULL){
 		widget->requested_size = *requested_size;
 	}
 	else {
 		widget->requested_size.height = 320;
 		widget->requested_size.width = 240;
 	}
	ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;

	if (color != NULL)
		toplevel->color = *color;

	if (border_width != NULL)
		toplevel->border_width = *border_width;
	else if (!toplevel->border_width)
		toplevel->border_width = 4;

	if (title != NULL) {
		toplevel->title = realloc(toplevel->title, strlen(*title)+1);
		strcpy(toplevel->title, *title);
	}

	if (closable != NULL)
		toplevel->closable = *closable;
	else if (!toplevel->closable)
		toplevel->closable = EI_TRUE;

	if (resizable != NULL)
		toplevel->resizable = *resizable;
	else if (!toplevel->resizable)
		toplevel->resizable = ei_axis_both;

	if (min_size != NULL)
		toplevel->min_size = *min_size;
	else if (!toplevel->min_size){
		(*(toplevel->min_size)).width = 160;
		(*(toplevel->min_size)).height = 120;
	}

	if (widget->content_rect->size.width < widget->requested_size.width){
		widget->content_rect->size.width = widget->requested_size.width;
	}
	if (widget->content_rect->size.height < widget->requested_size.height){
		widget->content_rect->size.height = widget->requested_size.height;
	}
}
