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
        //juste pour un test
        ei_point_t ici = {260,280};
        char* texte = "ATLAS";
        ei_color_t col = {0x00,0x00,0xff,0xff};
        //fin du test
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
                ei_rect_t unter = {low_first,size_unter};
                if (frame->relief == ei_relief_none) {
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
                fprintf(stdout, "pouloulou\n");
                ei_point_t aqui;
                ei_anchor_spot(frame->text_anchor,&(frame->widget),&aqui);
                ei_draw_text(surface,&aqui,*(frame->text),NULL, *(frame->text_color),clipper);
        }
        //gestion des enfants
        ei_widget_t* current_child = frame->widget.children_head;
        while (current_child){
                current_child->wclass->drawfunc(current_child, surface, pick_surface,current_child->content_rect);
                current_child = current_child->next_sibling;
        }
}

ei_linked_point_t* rounded_frame(ei_rect_t rectangle, int rayon, int nb_points){
        //On récupère les 4 points formant le rectangle
        ei_point_t top_left = {rectangle.top_left.x, rectangle.top_left.y};
        ei_point_t top_right = {rectangle.top_left.x + rectangle.size.width, rectangle.top_left.y};
        ei_point_t bot_left = {rectangle.top_left.x, rectangle.top_left.y + rectangle.size.height};
        ei_point_t bot_right = {rectangle.top_left.x + rectangle.size.width, rectangle.top_left.y + rectangle.size.height};
        //On détermine les 4 centres autour desquels on formera les arcs de cercles
        ei_point_t center_top_left = {top_left.x - rayon, top_left.y - rayon};
        ei_point_t center_top_right = {top_right.x - rayon, top_right.y - rayon};
        ei_point_t center_bot_left = {bot_left.x - rayon, bot_left.y - rayon};
        ei_point_t center_bot_right = {bot_right.x - rayon, bot_right.y - rayon};
        //On récupère les points formant les arcs de cercles de chaque coin du rectangle
        ei_extreme_linked_points_t* extreme_points_top_right = arc(center_top_right, rayon, 0, 90, nb_points);
        ei_extreme_linked_points_t* extreme_points_top_left = arc(center_top_left, rayon, 90, 180, nb_points);
        ei_extreme_linked_points_t* extreme_points_bot_left = arc(center_bot_left, rayon, 180, 270, nb_points);
        ei_extreme_linked_points_t* extreme_points_bot_right = arc(center_bot_right, rayon, 270, 360, nb_points);
        //On relie tous les points dans l'ordre
        extreme_points_top_left->tail_point->next = extreme_points_bot_left->head_point;
        extreme_points_bot_left->tail_point->next = extreme_points_bot_right->head_point;
        extreme_points_bot_right->tail_point->next = extreme_points_top_right->head_point;
        //On retourne alors le premier point de la chaine formée
        return extreme_points_top_left->head_point;
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
