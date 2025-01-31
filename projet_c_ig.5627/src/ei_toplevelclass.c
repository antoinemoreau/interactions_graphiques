#include "ei_toplevelclass.h"
#include "ei_toplevel.h"
#include "ei_draw_utils.h"
#include "ei_types.h"
#include "ei_button.h"
#include "ei_buttonclass.h"
#include "ei_utils.h"
#include "ei_placer.h"
#include <stdlib.h>
#include <stdio.h>

ei_bool_t closing(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
        ei_widget_destroy(ei_find_widget(widget->pick_id - 1, widget->parent));
        ei_widget_destroy(widget);
        destroy = EI_TRUE;
        return EI_TRUE;
}

static ei_linked_point_t *points_list(ei_rect_t rectangle)
{
        ei_linked_point_t *bot_left = calloc(1, sizeof(ei_linked_point_t));
        bot_left->point.x = rectangle.top_left.x;
        bot_left->point.y = rectangle.top_left.y + rectangle.size.height;

        ei_linked_point_t *bot_right = calloc(1, sizeof(ei_linked_point_t));
        bot_right->point.x = rectangle.top_left.x + rectangle.size.width;
        bot_right->point.y = rectangle.top_left.y + rectangle.size.height;
        bot_right->next = bot_left;

        ei_linked_point_t *top_right = calloc(1, sizeof(ei_linked_point_t));
        top_right->point.x = rectangle.top_left.x + rectangle.size.width;
        top_right->point.y = rectangle.top_left.y;
        top_right->next = bot_right;

        ei_linked_point_t *top_left = calloc(1, sizeof(ei_linked_point_t));
        top_left->point = rectangle.top_left;
        top_left->next = top_right;

        return top_left;
}

static ei_button_t *closing_button(ei_toplevel_t *toplevel)
{
        // Define the button parameters
        int radius = 8;
        int diameter = 2 * radius;

        int button_border_width = 1;
        ei_color_t button_color = {0xa9, 0x11, 0x01, 0xff};
        ei_relief_t relief = ei_relief_raised;
        ei_size_t requested_size = {diameter, diameter};
        ei_callback_t button_closing = closing;
        int button_x = toplevel->border_width + 5 * button_border_width;
        int button_y = 2 * toplevel->border_width + 5 * button_border_width;
        int button_width = diameter;
        int button_height = diameter;

        // Creates the close button and sets its parameters
        ei_widget_t *button_widget = ei_widget_create("button", toplevel->widget.parent);
        ei_button_t *button = (ei_button_t *)button_widget;

        ei_button_configure(button_widget, &requested_size, &button_color,
                            &button_border_width, &radius, &relief, NULL, NULL, NULL, NULL,
                            NULL, NULL, NULL, NULL, NULL);
        ei_place(button_widget, NULL, &button_x, &button_y, &button_width, &button_height, NULL, NULL, NULL, NULL);
        ei_bind(ei_ev_mouse_buttonup, button_widget, NULL, button_closing, NULL);

        return button;
}

void *ei_toplevel_allocfunc()
{
        return calloc(1, sizeof(ei_toplevel_t));
}

void ei_toplevel_releasefunc(ei_widget_t *widget)
{
        ei_toplevel_t* toplevel = (ei_toplevel_t*)widget;
        // No need to release the close button because it is the next sibling of the toplevel.
        if (toplevel->title)
                free(toplevel->title);
        if(toplevel->min_size){
                free(toplevel->min_size);
        }

}

void ei_toplevel_drawfunc(struct ei_widget_t *widget,
                          ei_surface_t surface,
                          ei_surface_t pick_surface,
                          ei_rect_t *clipper)
{

        ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
        ei_axis_set_t resizable = toplevel->resizable;
        ei_bool_t closable = toplevel->closable;

        ei_color_t color = toplevel->color;
        int border_width = toplevel->border_width;

        // Title
        ei_point_t text_spot = {toplevel->widget.screen_location.top_left.x + toplevel->border_width + \
                                2 * (toplevel->close_button->widget.requested_size.width), \
                                toplevel->widget.screen_location.top_left.y + toplevel->border_width};
        ei_size_t text_size;
        hw_text_compute_size(toplevel->title, ei_default_font, &(text_size.width), &(text_size.height));

        // Clipping the toplevel with the content rectangle of his parent
        toplevel->widget.content_rect->size.width = toplevel->widget.screen_location.size.width - 2 * toplevel->border_width;
        toplevel->widget.content_rect->size.height = toplevel->widget.screen_location.size.height - text_size.height - 2 * border_width;
        ei_rect_t intersection = {toplevel->widget.screen_location.top_left, toplevel->widget.screen_location.size};
        ei_intersection_rectangle(clipper, &(toplevel->widget.screen_location), &intersection);

        toplevel->widget.content_rect->top_left.x = toplevel->widget.screen_location.top_left.x + border_width;
        toplevel->widget.content_rect->top_left.y = toplevel->widget.screen_location.top_left.y + text_size.height + border_width;

        ei_rect_t interieur;
        ei_intersection_rectangle(clipper, toplevel->widget.content_rect, &interieur);

        // Creating the outer polygon and round its top corners
        int nb_points = 10;
        int rayon = 20;
        ei_point_t center_top_left = {toplevel->widget.screen_location.top_left.x + rayon, toplevel->widget.screen_location.top_left.y + rayon};
        ei_point_t center_top_right = {toplevel->widget.screen_location.top_left.x - rayon + toplevel->widget.screen_location.size.width, toplevel->widget.screen_location.top_left.y + rayon};
        ei_extreme_linked_points_t *arc_top_left = arc(center_top_left, rayon, 90.0, 180.0, nb_points);
        ei_extreme_linked_points_t *arc_top_right = arc(center_top_right, rayon, 0.0, 90.0, nb_points);

        ei_point_t point_bot_left = {toplevel->widget.screen_location.top_left.x, toplevel->widget.screen_location.top_left.y + toplevel->widget.screen_location.size.height};
        ei_point_t point_bot_right = {toplevel->widget.screen_location.top_left.x + toplevel->widget.screen_location.size.width, toplevel->widget.screen_location.top_left.y + toplevel->widget.screen_location.size.height};

        ei_linked_point_t *bot_right = calloc(1, sizeof(ei_linked_point_t));
        bot_right->point = point_bot_right;

        ei_linked_point_t *bot_left = calloc(1, sizeof(ei_linked_point_t));
        bot_left->point = point_bot_left;
        bot_left->next = bot_right;
        ei_linked_point_t *exter_first_point = arc_top_right->head_point;
        arc_top_right->tail_point->next = arc_top_left->head_point;
        arc_top_left->tail_point->next = bot_left;

        ei_draw_polygon(surface, exter_first_point, color, &intersection);


        ei_color_t inter_color = {0xff, 0xff, 0xff, 0x60};

        // Creating the inner polygon (represents the content rectangle)
        ei_linked_point_t *inter_first_point = points_list(*(widget->content_rect));
        ei_draw_polygon(surface, inter_first_point, inter_color, &interieur);
        ei_free_polygon(&inter_first_point);

        if (resizable != ei_axis_none)
        {
                // Creating the clickable square on bottom-right (toplevel resizer)
                ei_rect_t resize_rect;
                resize_rect.top_left.x = toplevel->widget.screen_location.top_left.x + toplevel->widget.screen_location.size.width - 4 * border_width;
                resize_rect.top_left.y = toplevel->widget.screen_location.top_left.y + toplevel->widget.screen_location.size.height - 4 * border_width;
                resize_rect.size.width = 4 * border_width;
                resize_rect.size.height = 4 * border_width;
                ei_linked_point_t *resize_rectangle = points_list(resize_rect);
                ei_draw_polygon(surface, resize_rectangle, color, &intersection);
                ei_free_polygon(&resize_rectangle);
        }

        if (pick_surface)
        {
                ei_draw_polygon(pick_surface, exter_first_point, *(widget->pick_color), &intersection);
                ei_free_polygon(&exter_first_point);
        }
        if (closable == EI_TRUE)
        {
                // Drawing the close button on top left
                toplevel->close_button->widget.screen_location.top_left.x = widget->screen_location.top_left.x + toplevel->border_width + \
                                                                                toplevel->close_button->border_width * 5;
                toplevel->close_button->widget.screen_location.top_left.y = widget->screen_location.top_left.y + toplevel->border_width * 2 + \
                                                                                toplevel->close_button->border_width * 5;
                toplevel->close_button->widget.screen_location.size.width = ((ei_widget_t *)toplevel->close_button)->requested_size.width;
                toplevel->close_button->widget.screen_location.size.height = ((ei_widget_t *)toplevel->close_button)->requested_size.height;
                ei_widget_t *button_widget = (ei_widget_t *)toplevel->close_button;
                ei_button_drawfunc(button_widget, surface, pick_surface, &intersection);
        }

        if (toplevel->title && strcmp(toplevel->title, "") != 0)
        {
                ei_rect_t title_rect = {text_spot, text_size};
                ei_point_t aqui;
                ei_anchor_spot(ei_anc_none, &text_size, &title_rect, &aqui);
                ei_draw_text(surface, &aqui, toplevel->title, ei_default_font, ei_font_default_color, &intersection);
        }

        // Free polygons
        free(arc_top_right);
        free(arc_top_left);
        free(exter_first_point);
        free(inter_first_point);
}

void ei_toplevel_setdefaultsfunc(struct ei_widget_t *widget)
{
        ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
        toplevel->widget = *widget;
        toplevel->color = ei_default_background_color;
        toplevel->border_width = 4;
        toplevel->title = malloc(sizeof("Toplevel"));
        strcpy(toplevel->title, "Toplevel");
        toplevel->closable = EI_TRUE;
        toplevel->resizable = ei_axis_both;
        toplevel->min_size = NULL;
        toplevel->close_button = closing_button(toplevel);
}

void ei_toplevel_geomnotifyfunc(struct ei_widget_t *widget, ei_rect_t rect)
{
        ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
        ei_size_t text_size;
        hw_text_compute_size(toplevel->title, ei_default_font, &text_size.width, &text_size.height);
        if (widget->screen_location.size.width - 2 * toplevel->border_width >= toplevel->min_size->width)
        {
                widget->content_rect->size.width = widget->screen_location.size.width - 2 * toplevel->border_width;
                widget->screen_location.size.width = rect.size.width + 2 * toplevel->border_width;
        }
        if (widget->screen_location.size.height - 2 * toplevel->border_width - text_size.height >= toplevel->min_size->height)
        {
                widget->screen_location.size.height = rect.size.height + text_size.height + 2 * toplevel->border_width;
                widget->content_rect->size.height = widget->screen_location.size.height - text_size.height - 2 * toplevel->border_width;
        }

        widget->content_rect->top_left.x = widget->screen_location.top_left.x + toplevel->border_width;
        widget->content_rect->top_left.y = widget->screen_location.top_left.y + text_size.height + toplevel->border_width;
}
