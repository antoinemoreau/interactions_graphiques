#include "ei_draw.h"
#include "ei_widget.h"
#include <stdio.h>

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
                (*anchor_position).x = (int) (widget->screen_location->top_left.x +  widget->screen_location.size.width/2);
                (*anchor_position).y = (int) (widget->screen_location->top_left.y +  widget->screen_location.size.height/2);
        }
        else if (anchor == ei_anc_north){
                (*anchor_position).x = (int) (widget->screen_location->top_left.x +  widget->screen_location.size.width/2);
                (*anchor_position).y = widget->screen_location->top_left.y;
        }
        else if (anchor == ei_anc_northeast){
                (*anchor_position).x = (int) (widget->screen_location->top_left.x +  widget->screen_location.size.width);
                (*anchor_position).y = widget->screen_location->top_left.y;
        }
        else if (anchor == ei_anc_east){
                (*anchor_position).x = (int) (widget->screen_location->top_left.x +  widget->screen_location.size.width);
                (*anchor_position).y = (int) (widget->screen_location->top_left.y +  widget->screen_location.size.height/2);
        }
        else if (anchor == ei_anc_southeast){
                (*anchor_position).x = (int) (widget->screen_location->top_left.x +  widget->screen_location.size.width);
                (*anchor_position).y = (int) (widget->screen_location->top_left.y +  widget->screen_location.size.height);
        }
        else if (anchor == ei_anc_south){
                (*anchor_position).y = (int) (widget->screen_location->top_left.y +  widget->screen_location.size.height);
                (*anchor_position).x = (int) (widget->screen_location->top_left.x +  widget->screen_location.size.width/2);
        }
        else if (anchor == ei_anc_southwest){
                (*anchor_position).y = (int) (widget->screen_location->top_left.y +  widget->screen_location.size.height);
                (*anchor_position).x = widget->screen_location->top_left.x;
        }
        else if (anchor == ei_anc_west){
                (*anchor_position).x = widget->screen_location->top_left.x;
                (*anchor_position).y = (int) (widget->screen_location->top_left.y +  widget->screen_location.size.height/2);
        }
        else if (anchor == ei_anc_northwest){
                (*anchor_position).x = widget->screen_location->top_left.x;
                (*anchor_position).y = widget->screen_location->top_left.y;
        }
}
