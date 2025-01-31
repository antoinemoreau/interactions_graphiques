#include "ei_buttonclass.h"
#include "ei_types.h"
#include "ei_button.h"
#include "ei_draw_utils.h"
#include "ei_draw.h"
#include "ei_application.h"
#include <stdlib.h>
#include <stdio.h>
void *ei_button_allocfunc()
{
        return calloc(1, sizeof(ei_button_t));
}

void            ei_button_releasefunc           (ei_widget_t* widget) {
        ei_button_t* button = (ei_button_t*)widget;
        if (button->text)
                free(button->text);
        // if (button->text_font)
                //hw_text_font_free(button->text_font);
        if (button->img_rect)
                free(button->img_rect);
        // if (button->img)
        //         hw_surface_free(button->img);
}

ei_linked_point_t* rounded_frame(ei_rect_t rectangle, int rayon, int nb_points, int partie){
        if (partie != 0 && partie != 1 && partie != 2){
                fprintf(stderr, "Selected part does not exist : \n --> 0 = low part \n --> 1 = high part \n --> 2 = all \n" );
                exit(1);
        }
        // Collecting the four points forming the rectangle
        ei_point_t top_left = {rectangle.top_left.x, rectangle.top_left.y};
        ei_point_t top_right = {rectangle.top_left.x + rectangle.size.width, rectangle.top_left.y};
        ei_point_t bot_left = {rectangle.top_left.x, rectangle.top_left.y + rectangle.size.height};
        ei_point_t bot_right = {rectangle.top_left.x + rectangle.size.width, rectangle.top_left.y + rectangle.size.height};
        // Computing the four centers of the arcs
        ei_point_t center_top_left = {top_left.x + rayon, top_left.y + rayon};
        ei_point_t center_top_right = {top_right.x - rayon, top_right.y + rayon};

        ei_point_t center_bot_left = {bot_left.x + rayon, bot_left.y - rayon};
        ei_point_t center_bot_right = {bot_right.x - rayon, bot_right.y - rayon};
        // Collecting the point forming the arcs of each corner of the rectangle
        ei_extreme_linked_points_t* extreme_points_top_right_low = arc(center_top_right, rayon, 0.0, 45.0, nb_points/2);
        ei_extreme_linked_points_t* extreme_points_top_right_high = arc(center_top_right, rayon, 45.0, 90.0, nb_points/2);
        ei_extreme_linked_points_t* extreme_points_top_left = arc(center_top_left, rayon, 90.0, 180.0, nb_points);
        ei_extreme_linked_points_t* extreme_points_bot_left_high = arc(center_bot_left, rayon, 180.0, 225.0, nb_points/2);
        ei_extreme_linked_points_t* extreme_points_bot_left_low = arc(center_bot_left, rayon, 225.0, 270.0, nb_points/2);
        ei_extreme_linked_points_t* extreme_points_bot_right = arc(center_bot_right, rayon, 270.0, 360.0, nb_points);

        // Declaration of the first point of the linked point list
        ei_linked_point_t* first_point = NULL;

        /* We link the differents points of the section:
                2 : The all part
                1 : The top part
                0 : The bottom part
        */
        if (partie == 2) {
                extreme_points_top_left->tail_point->next = extreme_points_bot_left_high->head_point;
                extreme_points_bot_left_high->tail_point->next = extreme_points_bot_left_low->head_point;
                extreme_points_bot_left_low->tail_point->next = extreme_points_bot_right->head_point;
                extreme_points_bot_right->tail_point->next = extreme_points_top_right_low->head_point;
                extreme_points_top_right_low->tail_point->next = extreme_points_top_right_high->head_point;
                extreme_points_top_right_high->tail_point->next = NULL;
                first_point = extreme_points_top_left->head_point;
        }
        else if (partie == 0)
        {
                extreme_points_bot_left_low->tail_point->next = extreme_points_bot_right->head_point;
                extreme_points_bot_right->tail_point->next = extreme_points_top_right_low->head_point;
                extreme_points_top_right_low->tail_point->next = NULL;
                first_point = extreme_points_bot_left_low->head_point;
                ei_free_polygon(&extreme_points_top_left->head_point);
                ei_free_polygon(&extreme_points_bot_left_high->head_point);
                ei_free_polygon(&extreme_points_top_right_high->head_point);
        }
        else
        {
                extreme_points_top_right_high->tail_point->next = extreme_points_top_left->head_point;
                extreme_points_top_left->tail_point->next = extreme_points_bot_left_high->head_point;
                extreme_points_bot_left_high->tail_point->next = NULL;
                first_point = extreme_points_top_right_high->head_point;
                ei_free_polygon(&extreme_points_bot_left_low->head_point);
                ei_free_polygon(&extreme_points_bot_right->head_point);
                ei_free_polygon(&extreme_points_top_right_low->head_point);
        }
        free(extreme_points_top_left);
        free(extreme_points_top_right_low);
        free(extreme_points_top_right_high);
        free(extreme_points_bot_left_low);
        free(extreme_points_bot_left_high);
        free(extreme_points_bot_right);
        // Returning the first point of the chain
        return first_point;
}

void ei_button_drawfunc(ei_widget_t *widget,
                        ei_surface_t surface,
                        ei_surface_t pick_surface,
                        ei_rect_t *clipper)
{

        ei_button_t* button = (ei_button_t*) widget;
        if (widget->screen_location.size.height == 0 && widget->screen_location.size.width == 0) {
                if (button->text) {
                        // If the button is of the size (zero, zero) and does have text we do draw it so that it correspond to the text size
                        hw_text_compute_size(button->text, button->text_font, &(widget->screen_location.size.width), &(widget->screen_location.size.height));
                } else {
                        // If the button is of the size (zero, zero) and does not have text we do not draw it
                        fprintf(stderr, "Warning : button size is (0,0).\n");
                        return;
                }
        }

        ei_rect_t inter = {button->widget.screen_location.top_left, button->widget.screen_location.size};
        ei_intersection_rectangle(clipper, &(button->widget.screen_location), &inter);
        // button->widget.screen_location.size.width = inter.size.width;
        // button->widget.screen_location.size.height = inter.size.height;
        // button->widget.screen_location.top_left.x = inter.top_left.x;
        // button->widget.screen_location.top_left.y = inter.top_left.y;
        // button->widget.content_rect->size.width = inter.size.width;
        // button->widget.content_rect->size.height = inter.size.height;
        // button->widget.content_rect->top_left.x = inter.top_left.x;
        // button->widget.content_rect->top_left.y = inter.top_left.y;

        int border = button->border_width;

        // Determining the three part we have to color
        int nb_points = 10;
        ei_color_t light_color;
        ei_color_t dark_color;
        ei_compute_color(button->color, &light_color, 1.2);
        ei_compute_color(button->color, &dark_color, 0.5);

        ei_point_t point_top_right_big_square = {button->widget.screen_location.top_left.x + button->widget.screen_location.size.width - button->widget.screen_location.size.height / 2,
                                                 button->widget.screen_location.top_left.y + button->widget.screen_location.size.height / 2};
        ei_point_t point_bot_left_big_square = {button->widget.screen_location.top_left.x + button->widget.screen_location.size.height / 2,
                                                button->widget.screen_location.top_left.y + button->widget.screen_location.size.height / 2};

        ei_linked_point_t *bot_left_big_square = calloc(1, sizeof(ei_linked_point_t));
        ei_linked_point_t *top_right_big_square = calloc(1, sizeof(ei_linked_point_t));

        top_right_big_square->point = point_top_right_big_square;
        bot_left_big_square->point = point_bot_left_big_square;

        // Computing the polygon of the low that have to be colored
        ei_linked_point_t* low_part = rounded_frame(button->widget.screen_location, button->corner_radius, nb_points, 0);
        top_right_big_square->next = bot_left_big_square;
        bot_left_big_square->next = low_part;


        // Computing the polygon of the high that have to be colored
        ei_linked_point_t* high_part = rounded_frame(button->widget.screen_location, button->corner_radius, nb_points, 1);
        bot_left_big_square->next = top_right_big_square;
        top_right_big_square->next = high_part;

        // Drawing of the hight part and low part with rigth the relief color according to the relief parametre value
        if (button->relief == ei_relief_raised) {
                ei_draw_polygon(surface, high_part, light_color, &inter);
                ei_draw_polygon(surface, low_part, dark_color, &inter);
        }
        else if(button->relief == ei_relief_sunken){
                ei_draw_polygon(surface, high_part, dark_color, &inter);
                ei_draw_polygon(surface, low_part, light_color, &inter);
        }
        // Resizing the interior rectangle relatively the border width
        inter.top_left.x += border;
        inter.top_left.y += border;
        inter.size.width -= 2 * border;
        inter.size.height -= 2 * border;

        // Freeing the intermediary variables
        free(bot_left_big_square);
        free(top_right_big_square);

        // Drawing the all inside of the button
        ei_linked_point_t* all_part = rounded_frame(inter, button->corner_radius, nb_points, 2);
        ei_draw_polygon(surface, all_part, button->color, &inter);
        ei_free_polygon(&all_part);

        // Update of the picking surface
        if (pick_surface) {
                ei_linked_point_t* pick_poly = rounded_frame(button->widget.screen_location, button->corner_radius, nb_points, 2);
                ei_draw_polygon(pick_surface, pick_poly, *(button->widget.pick_color), &button->widget.screen_location);
                ei_free_polygon(&pick_poly);
        }
        // Freeing the polygons used for the drawings
        ei_free_polygon(&high_part);
        ei_free_polygon(&low_part);

        // Display of the button text, if it have one
        if (button->text && strcmp(button->text, "") != 0) {
                ei_point_t pos_texte;
                ei_size_t size_texte;
                hw_text_compute_size(button->text, button->text_font, &(size_texte.width), &(size_texte.height));
                ei_anchor_spot(button->text_anchor, &size_texte, &inter, &pos_texte);
                // If the button is sunken we have to draw in the button according to the relief
                if(button->relief == ei_relief_sunken) {
                        pos_texte.x += border/2;
                        pos_texte.y += border/2;
                }
                ei_draw_text(surface, &pos_texte, button->text, button->text_font, button->text_color, &inter);
        // If the button does not have a text but have an image we display it
        } else if (button->img) {
                ei_point_t pos_img;
                ei_size_t size_img;
                ei_rect_t rect_surface_img;
                // Checking if the user have provide us the recangle corresponding to the image
                // If not we set it according to the rectangle of the image surface
                if (button->img_rect) {
                        rect_surface_img = *(button->img_rect);
                        size_img = button->img_rect->size;
                }
                else
                {
                        rect_surface_img = hw_surface_get_rect(button->img);
                        size_img = rect_surface_img.size;
                }
                //computing the postion of the image according to its anchor
                ei_anchor_spot(button->img_anchor, &size_img, &inter, &pos_img);
                ei_rect_t rect_img = {pos_img, size_img};

                hw_surface_lock(button->img);
                ei_bool_t alpha_img = hw_surface_has_alpha(button->img);
                ei_rect_t surface_rect = hw_surface_get_rect(ei_app_root_surface());

                ei_rect_t clipper_img;
                ei_intersection_rectangle(&inter, &surface_rect, &clipper_img);

                ei_rect_t dest_final;
                ei_intersection_rectangle(&clipper_img, &rect_img, &dest_final);
                //settting the top_left point of the image rectangle at the right location
                rect_img.top_left.x = rect_surface_img.top_left.x + dest_final.top_left.x - pos_img.x;
                rect_img.top_left.y = rect_surface_img.top_left.y;
                rect_img.size = dest_final.size;
                //copying the rect image content on screen
                ei_copy_surface(surface, &dest_final, button->img, &rect_img, alpha_img);
                hw_surface_unlock(button->img);
        }
}

void ei_button_setdefaultsfunc(ei_widget_t *widget)
{
        ei_button_t *button = (ei_button_t *)widget;
        button->widget = *widget;
        button->color = ei_default_background_color;
        button->border_width = k_default_button_border_width;
        button->corner_radius = k_default_button_corner_radius;
        button->relief = ei_relief_raised;
        button->text = NULL;
        button->text_font = ei_default_font;
        button->text_color = ei_font_default_color;
        button->text_anchor = ei_anc_center;
        button->img = NULL;
        button->img_rect = NULL;
        button->img_anchor = ei_anc_center;
        button->callback = NULL;
        button->user_param = NULL;
}

void ei_button_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect)
{
        *widget->content_rect = rect;
}
