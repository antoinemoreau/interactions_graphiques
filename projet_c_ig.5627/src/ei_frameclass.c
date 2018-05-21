#include "ei_frameclass.h"
#include "ei_types.h"
#include "ei_frame.h"
#include "ei_draw_utils.h"
#include <stdlib.h>
#include <stdio.h>

void* ei_frame_allocfunc () {
        return calloc(1, sizeof(ei_frame_t));
}


void ei_frame_releasefunc (struct ei_widget_t* widget) {
        //free(widget);
}

void ei_frame_drawfunc      (ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {

        ei_frame_t* frame = (ei_frame_t*) widget;
        ei_rect_t unter = {clipper->top_left,clipper->size};
        if (frame->border_width >0) {
                ei_size_t size_unter = {frame->widget.screen_location.size.width-2*frame->border_width,frame->widget.screen_location.size.height-2*frame->border_width};
                ei_color_t light_color;
                ei_color_t dark_color;
                ei_compute_color(*frame->color,&light_color,1.2);
                ei_compute_color(*frame->color,&dark_color,0.5);
                ei_point_t top_first = frame->widget.screen_location.top_left;
                ei_point_t top_right = {top_first.x + frame->widget.screen_location.size.width,top_first.y};
                ei_point_t right = {top_right.x - frame->border_width, top_right.y + frame->border_width};
                ei_point_t low_first = {top_first.x + frame->border_width,right.y};
                ei_point_t bot_last = {top_first.x,top_first.y + frame->widget.screen_location.size.height};
                ei_point_t last = {low_first.x,bot_last.y - frame->border_width};
                ei_point_t bot_bot = {top_first.x + frame->widget.screen_location.size.width,top_first.y+ frame->widget.screen_location.size.height};
                ei_point_t bot_in = {bot_bot.x - frame->border_width, bot_bot.y - frame->border_width};
                //creation du polygone du haut
                ei_linked_point_t top_poly_last = {last,NULL};
                ei_linked_point_t top_poly_bot = {bot_last,&top_poly_last};
                ei_linked_point_t top_poly_top = {top_first, &top_poly_bot};
                ei_linked_point_t top_poly_top_right = {top_right, &top_poly_top};
                ei_linked_point_t top_poly_right = {right, &top_poly_top_right};
                ei_linked_point_t top_poly_first = {low_first, &top_poly_right};
                //creation du polygone du bas
                ei_linked_point_t bot_poly_last = {last,NULL};
                ei_linked_point_t bot_poly_bot = {bot_last,&bot_poly_last};
                ei_linked_point_t bot_poly_bot_bas = {bot_bot,&bot_poly_bot};
                ei_linked_point_t bot_poly_right = {top_right,&bot_poly_bot_bas};
                ei_linked_point_t bot_poly_right_in = {right,&bot_poly_right};
                ei_linked_point_t bot_poly_first = {bot_in,&bot_poly_right_in};
                unter.top_left = low_first;
                unter.size = size_unter;
                if (frame->relief == ei_relief_none) {
                        unter.top_left = clipper->top_left;
                        unter.size = clipper->size;
                        ei_fill(surface,frame->color,clipper);
                }else if (frame->relief == ei_relief_raised) {
                        ei_draw_polygon(surface,&top_poly_first,light_color,clipper);
                        ei_draw_polygon(surface,&bot_poly_first,dark_color,clipper);
                        ei_fill(surface, frame->color,&unter);
                }else if(frame->relief == ei_relief_sunken){
                        ei_draw_polygon(surface,&top_poly_first,dark_color,clipper);
                        ei_draw_polygon(surface,&bot_poly_first,light_color,clipper);
                        ei_fill(surface, frame->color,&unter);
                }
        }else{
                ei_fill(surface, frame->color, clipper);
        }
        //ei_draw_text(surface,&ici,texte,NULL,col,NULL);
        if (pick_surface) {
                hw_surface_lock(pick_surface);
                ei_fill(pick_surface,frame->widget.pick_color,clipper);
                hw_surface_unlock(pick_surface);
        }
        if (frame->text) {
                ei_point_t aqui;
                ei_anchor_spot(frame->text_anchor,&(frame->widget),&aqui);
                ei_draw_text(surface,&aqui,*(frame->text),NULL, *(frame->text_color),&unter);
        }else{
                if(frame->img){
                        ei_point_t aqui_image;
                        ei_anchor_spot(frame->img_anchor,&(frame->widget),&aqui_image);
                }
        }
        //gestion des enfants
        ei_widget_t* current_child = frame->widget.children_head;
        while (current_child){
                current_child->wclass->drawfunc(current_child, surface, pick_surface, current_child->content_rect);
                current_child = current_child->next_sibling;
        }
}

void ei_frame_setdefaultsfunc (ei_widget_t* widget) {
        ei_frame_t* frame = (ei_frame_t*) widget;
        frame->widget = *widget;
        frame->color = &ei_default_background_color;
        frame->border_width = 0;
        frame->relief = ei_relief_none;
        frame->text = NULL;
        frame->text_font = ei_default_font;
        frame->text_color = &ei_font_default_color;
        frame->text_anchor = ei_anc_center;
        frame->img = NULL;
        frame->img_rect = NULL;
        frame->img_anchor = ei_anc_center;

}

void ei_frame_geomnotifyfunc (struct ei_widget_t* widget, ei_rect_t rect) {

}
