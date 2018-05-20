#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_draw_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void ei_compute_color(ei_color_t initiale, ei_color_t* res, float variation){
        int rouge = (int) initiale.red*variation ;
        int bleu = (int) initiale.blue*variation;
        int vert = (int) initiale.green*variation;
        if (rouge < 255) {
                res->red = rouge;
        } else {
                res->red = 255;
        }
        if (bleu < 255) {
                res->blue = bleu;
        } else {
                res->blue = 255;
        }
        if (vert < 255) {
                res->green = vert;
        } else {
                res->green = 255;
        }
        res->alpha = initiale.alpha;
}

static ei_rect_t* rectangle_abs_min(ei_rect_t* clipper, ei_rect_t* sec_rect){
        if (clipper->top_left.x <= sec_rect->top_left.x){
                return clipper;
        }
        else{
                return sec_rect;
        }
}

static ei_rect_t* rectangle_ord_min(ei_rect_t* clipper, ei_rect_t* sec_rect){
        if (clipper->top_left.y <= sec_rect->top_left.y){
                return clipper;
        }
        else{
                return sec_rect;
        }
}

void ei_intersection_rectangle(ei_rect_t* clipper, ei_rect_t* sec_rect, ei_rect_t* dest){
        ei_rect_t* left_rect = rectangle_abs_min(clipper, sec_rect);
        ei_rect_t* right_rect = NULL;
        ei_rect_t* top_rect = rectangle_ord_min(clipper, sec_rect);
        ei_rect_t* bot_rect = NULL;
        if (left_rect == clipper) {
                right_rect = sec_rect;
        }
        else {
                right_rect = clipper;
        }
        if (top_rect == clipper){
                bot_rect = sec_rect;
        }
        else{
                bot_rect = clipper;
        }
        //points du rectangle de destination
        ei_point_t top_dest;
        ei_size_t dest_size;

        if ( (left_rect->top_left.x + left_rect->size.width <= right_rect->top_left.x) || top_rect->top_left.y + top_rect->size.height <= bot_rect->top_left.y){
                top_dest.x = 0;
                top_dest.y = 0;
                dest_size.width = 0;
                dest_size.height = 0;
        }
        else{
                top_dest.y = bot_rect->top_left.y;
                top_dest.x = right_rect->top_left.x;

                dest_size.width = left_rect->top_left.x + left_rect->size.width - top_dest.x;
                dest_size.height = top_rect->top_left.y + top_rect->size.height - top_dest.y;
        }
        dest->top_left = top_dest;
        dest->size = dest_size;
        fprintf(stdout, "x: %d y: %d widht: %d height: %d\n", dest->top_left.x, dest->top_left.y, dest->size.width, dest->size.height);
}

void ei_anchor_spot(ei_anchor_t anchor, ei_widget_t* widget, ei_point_t* anchor_position){
        /*
        Renvoi la position du point choisi avec anchor_position

        Condition : le widget a un geometry manager et anchor différent de none
        */
        if (strcmp(widget->geom_params->manager->name, "placer") != 0){
                fprintf(stderr, "Geometry manager is not \"placer\" \n");
                exit(1);
        }
        else if (anchor == ei_anc_none){
                anchor = ei_anc_northwest;
        }


        if (anchor == ei_anc_center){
                (*anchor_position).x = (int) (widget->screen_location.top_left.x +  widget->screen_location.size.width/2);
                (*anchor_position).y = (int) (widget->screen_location.top_left.y +  widget->screen_location.size.height/2);
        }
        else if (anchor == ei_anc_north){
                (*anchor_position).x = (int) (widget->screen_location.top_left.x +  widget->screen_location.size.width/2);
                (*anchor_position).y = widget->screen_location.top_left.y;
        }
        else if (anchor == ei_anc_northeast){
                (*anchor_position).x = (int) (widget->screen_location.top_left.x +  widget->screen_location.size.width);
                (*anchor_position).y = widget->screen_location.top_left.y;
        }
        else if (anchor == ei_anc_east){
                (*anchor_position).x = (int) (widget->screen_location.top_left.x +  widget->screen_location.size.width);
                (*anchor_position).y = (int) (widget->screen_location.top_left.y +  widget->screen_location.size.height/2);
        }
        else if (anchor == ei_anc_southeast){
                (*anchor_position).x = (int) (widget->screen_location.top_left.x +  widget->screen_location.size.width);
                (*anchor_position).y = (int) (widget->screen_location.top_left.y +  widget->screen_location.size.height);
        }
        else if (anchor == ei_anc_south){
                (*anchor_position).y = (int) (widget->screen_location.top_left.y +  widget->screen_location.size.height);
                (*anchor_position).x = (int) (widget->screen_location.top_left.x +  widget->screen_location.size.width/2);
        }
        else if (anchor == ei_anc_southwest){
                (*anchor_position).y = (int) (widget->screen_location.top_left.y +  widget->screen_location.size.height);
                (*anchor_position).x = widget->screen_location.top_left.x;
        }
        else if (anchor == ei_anc_west){
                (*anchor_position).x = widget->screen_location.top_left.x;
                (*anchor_position).y = (int) (widget->screen_location.top_left.y +  widget->screen_location.size.height/2);
        }
        else if (anchor == ei_anc_northwest){
                (*anchor_position).x = widget->screen_location.top_left.x;
                (*anchor_position).y = widget->screen_location.top_left.y;
        }
}

ei_extreme_linked_points_t* arc(ei_point_t center, int rayon, int angle_depart, int angle_fin, int nb_points){
        /*
        Retourne une chaine de points calculés pour formé un quart de cercle
        compris entre angle_depart et angle_fin
        */
        ei_point_t point = {center.x + rayon * cos(angle_depart), center.y + rayon * sin(angle_depart)};
        ei_linked_point_t* first_point = calloc(1, sizeof(ei_linked_point_t));
        int angle;
        first_point->point = point;
        first_point->next = NULL;
        ei_linked_point_t* previous = first_point;
        ei_linked_point_t* current = NULL;

        for (int i = 1; i <= nb_points + 1; i++){
                angle = angle_depart + i*(angle_fin - angle_depart)/(nb_points + 1);
                current = calloc(1, sizeof(ei_linked_point_t));
                point.x = center.x + rayon * cos(angle);
                point.y = center.y + rayon * sin(angle);
                current->point = point;
                current->next = NULL;
                previous->next = current;
                previous = current;
                current = current->next;
        }
        ei_extreme_linked_points_t* extreme_points = calloc(1, sizeof(ei_extreme_linked_points_t));
        extreme_points->head_point = first_point;
        extreme_points->tail_point = previous;
        return extreme_points;
}
