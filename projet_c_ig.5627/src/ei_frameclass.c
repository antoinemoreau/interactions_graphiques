#include "ei_frameclass.h"
#include "ei_types.h"
#include "ei_frame.h"
#include "ei_draw_utils.h"
#include "ei_draw.h"
#include <stdlib.h>
#include <stdio.h>

void *ei_frame_allocfunc()
{
        return calloc(1, sizeof(ei_frame_t));
}

void ei_frame_releasefunc(struct ei_widget_t *widget)
{
        ei_frame_t *frame = (ei_frame_t *)widget;
        if (frame->text)
                free(frame->text);
        if (frame->text_font)
                //hw_text_font_free(frame->text_font);
                if (frame->img)
                        hw_surface_free(frame->img);
        if (frame->img_rect)
                free(frame->img_rect);
}

void ei_frame_drawfunc(ei_widget_t *widget,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t *clipper)
{

        ei_frame_t *frame = (ei_frame_t *)widget;
        ei_rect_t inter = {frame->widget.screen_location.top_left, frame->widget.screen_location.size};
        ei_intersection_rectangle(clipper, &(frame->widget.screen_location), &inter);
        frame->widget.screen_location.size.width = inter.size.width;
        frame->widget.screen_location.size.height = inter.size.height;
        frame->widget.screen_location.top_left.x = inter.top_left.x;
        frame->widget.screen_location.top_left.y = inter.top_left.y;
        frame->widget.content_rect->top_left.x = inter.top_left.x;
        frame->widget.content_rect->top_left.y = inter.top_left.y;
        frame->widget.content_rect->size.width = inter.size.width;
        frame->widget.content_rect->size.height = inter.size.height;

        if (frame->border_width > 0)
        {

                ei_color_t light_color;
                ei_color_t dark_color;
                ei_compute_color(frame->color, &light_color, 1.2);
                ei_compute_color(frame->color, &dark_color, 0.5);
                ei_point_t top_first = inter.top_left;
                ei_point_t top_right = {top_first.x + inter.size.width, top_first.y};
                ei_point_t right = {top_right.x - frame->border_width, top_right.y + frame->border_width};
                ei_point_t low_first = {top_first.x + frame->border_width, right.y};
                ei_point_t bot_last = {top_first.x, top_first.y + inter.size.height};
                ei_point_t last = {low_first.x, bot_last.y - frame->border_width};
                ei_point_t bot_bot = {top_first.x + inter.size.width, top_first.y + inter.size.height};
                ei_point_t bot_in = {bot_bot.x - frame->border_width, bot_bot.y - frame->border_width};
                //creation du polygone du haut
                ei_linked_point_t top_poly_last = {last, NULL};
                ei_linked_point_t top_poly_bot = {bot_last, &top_poly_last};
                ei_linked_point_t top_poly_top = {top_first, &top_poly_bot};
                ei_linked_point_t top_poly_top_right = {top_right, &top_poly_top};
                ei_linked_point_t top_poly_right = {right, &top_poly_top_right};
                ei_linked_point_t top_poly_first = {low_first, &top_poly_right};
                //creation du polygone du bas
                ei_linked_point_t bot_poly_last = {last, NULL};
                ei_linked_point_t bot_poly_bot = {bot_last, &bot_poly_last};
                ei_linked_point_t bot_poly_bot_bas = {bot_bot, &bot_poly_bot};
                ei_linked_point_t bot_poly_right = {top_right, &bot_poly_bot_bas};
                ei_linked_point_t bot_poly_right_in = {right, &bot_poly_right};
                ei_linked_point_t bot_poly_first = {bot_in, &bot_poly_right_in};
                if (frame->relief == ei_relief_none)
                {
                        ei_fill(surface, &frame->color, &inter);
                }
                else if (frame->relief == ei_relief_raised)
                {
                        ei_draw_polygon(surface, &top_poly_first, light_color, &inter);
                        ei_draw_polygon(surface, &bot_poly_first, dark_color, &inter);
                        inter.top_left.x += frame->border_width;
                        inter.top_left.y += frame->border_width;
                        inter.size.width -= 2 * frame->border_width;
                        inter.size.height -= 2 * frame->border_width;
                        frame->widget.content_rect->top_left.x = inter.top_left.x;
                        frame->widget.content_rect->size.width = inter.size.width;
                        frame->widget.content_rect->size.height = inter.size.height;
                        frame->widget.content_rect->top_left.y = inter.top_left.y;
                        ei_fill(surface, &frame->color, &inter);
                }
                else if (frame->relief == ei_relief_sunken)
                {
                        ei_draw_polygon(surface, &top_poly_first, dark_color, &inter);
                        ei_draw_polygon(surface, &bot_poly_first, light_color, &inter);
                        inter.top_left.x += frame->border_width;
                        inter.top_left.y += frame->border_width;
                        inter.size.width -= 2 * frame->border_width;
                        inter.size.height -= 2 * frame->border_width;
                        frame->widget.content_rect->top_left.x = inter.top_left.x;
                        frame->widget.content_rect->size.width = inter.size.width;
                        frame->widget.content_rect->size.height = inter.size.height;
                        frame->widget.content_rect->top_left.y = inter.top_left.y;
                        ei_fill(surface, &frame->color, &inter);
                }
        }
        else
        {
                ei_fill(surface, &frame->color, &inter);
        }
        if (pick_surface)
        {
                ei_point_t top_first = inter.top_left;
                ei_point_t top_right = {top_first.x + inter.size.width, top_first.y};
                ei_point_t bot_bot = {top_first.x + inter.size.width, top_first.y + inter.size.height};
                ei_point_t bot_last = {top_first.x, top_first.y + inter.size.height};

                ei_linked_point_t top_exter = {top_first, NULL};
                ei_linked_point_t right_exter = {top_right, &top_exter};
                ei_linked_point_t bot_exter = {bot_bot, &right_exter};
                ei_linked_point_t left_exter = {bot_last, &bot_exter};
                ei_draw_polygon(pick_surface, &left_exter, *(frame->widget.pick_color), &inter);
        }
        if (frame->text && strcmp(frame->text, "") != 0)
        {
                ei_point_t aqui;
                ei_size_t size_texte = {0, 0};
                hw_text_compute_size(frame->text, frame->text_font, &(size_texte.width), &(size_texte.height));
                ei_anchor_spot(frame->text_anchor, &size_texte, &inter, &aqui);
                ei_draw_text(surface, &aqui, frame->text, NULL, frame->text_color, &inter);
        }
        else
        {
                if (frame->img)
                {
                        ei_point_t pos_img;
                        ei_size_t size_img;
                        ei_rect_t rect_surface_img;
                        if (frame->img_rect)
                        {
                                rect_surface_img = *(frame->img_rect);
                                size_img = frame->img_rect->size;
                        }
                        else
                        {
                                rect_surface_img = hw_surface_get_rect(frame->img);
                                size_img = rect_surface_img.size;
                        }
                        ei_anchor_spot(frame->img_anchor, &size_img, &inter, &pos_img);
                        ei_rect_t rect_img = {pos_img, size_img};

                        hw_surface_lock(frame->img);
                        ei_bool_t alpha_img = hw_surface_has_alpha(frame->img);
                        ei_rect_t surface_rect = hw_surface_get_rect(ei_app_root_surface());

                        ei_rect_t clipper_img;
                        ei_intersection_rectangle(&inter, &surface_rect, &clipper_img);

                        ei_rect_t dest_final;
                        ei_intersection_rectangle(&clipper_img, &rect_img, &dest_final);

                        rect_img.top_left.x = rect_surface_img.top_left.x + dest_final.top_left.x - pos_img.x;
                        rect_img.top_left.y = rect_surface_img.top_left.y;
                        rect_img.size = dest_final.size;
                        ei_copy_surface(surface, &dest_final, frame->img, &rect_img, alpha_img);
                        hw_surface_unlock(frame->img);
                }
        }
}

void ei_frame_setdefaultsfunc(ei_widget_t *widget)
{
        ei_frame_t *frame = (ei_frame_t *)widget;
        frame->widget = *widget;
        frame->color = ei_default_background_color;
        frame->border_width = 0;
        frame->relief = ei_relief_none;
        frame->text = NULL;
        frame->text_font = ei_default_font;
        frame->text_color = ei_font_default_color;
        frame->text_anchor = ei_anc_center;
        frame->img = NULL;
        frame->img_rect = NULL;
        frame->img_anchor = ei_anc_center;
}

void ei_frame_geomnotifyfunc(struct ei_widget_t *widget, ei_rect_t rect)
{
        return;
}
