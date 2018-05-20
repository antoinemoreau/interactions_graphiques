#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void ei_intersection_rectangle(ei_rect_t* clipper, ei_rect_t* sec_rect, ei_rect_t* dest){
        ei_point_t first_rect_x = clipper->top_left.x;
        ei_point_t first_rect_y = clipper->top_left.y;
        ei_point_t sec_rect_x = sec_rect->top_left.x;
        ei_point_t sec_rect_y = sec_rect->top_left.y;
        ei_point_t top_dest = {0,0};
        if (sec_rect_x =>first_rect_x || sec_rect_y => first_rect_y) {
                top_dest.x = first_rect_x;
                top_dest.y = first_rect_y;
        } else {
                top_dest.x = sec_rect_x;
                top_dest.y = sec_rect_y;
        }
        

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
