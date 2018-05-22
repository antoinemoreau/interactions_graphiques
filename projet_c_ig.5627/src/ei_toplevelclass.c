#include "ei_toplevelclass.h"
#include "ei_toplevel.h"
#include "ei_draw_utils.h"
#include "ei_types.h"
#include <stdlib.h>
#include <stdio.h>

static ei_linked_point_t* points_list (ei_rect_t rectangle){

        ei_linked_point_t* bot_left = calloc(1, sizeof(ei_linked_point_t));
        bot_left->point.x = rectangle.top_left.x;
        bot_left->point.y = rectangle.top_left.y + rectangle.size.height;

        ei_linked_point_t* bot_right = calloc(1, sizeof(ei_linked_point_t));
        bot_right->point.x = rectangle.top_left.x + rectangle.size.width;
        bot_right->point.y = rectangle.top_left.y + rectangle.size.height;
        bot_right->next = bot_left;

        ei_linked_point_t* top_right = calloc(1, sizeof(ei_linked_point_t));
        top_right->point.x = rectangle.top_left.x + rectangle.size.width;
        top_right->point.y = rectangle.top_left.y;
        top_right->next = bot_right;

        ei_linked_point_t* top_left = calloc(1, sizeof(ei_linked_point_t));
        top_left->point = rectangle.top_left;
        top_left->next = top_right;

        //Chainage = top_left->top_right->bot_right->bot_left
        return top_left;
}

void* ei_toplevel_allocfunc(){
        return calloc(1, sizeof(ei_toplevel_t));
}

void ei_toplevel_releasefunc(ei_widget_t* widget){
        ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
        free(toplevel);
}

void ei_toplevel_drawfunc (struct ei_widget_t* widget,
                                                        ei_surface_t surface,
                                                        ei_surface_t pick_surface,
                                                        ei_rect_t* clipper){

        ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
        ei_color_t color = *(toplevel->color);
        char** title = toplevel->title;
        int border_width = toplevel->border_width;
        ei_point_t toplevel_spot = toplevel->widget.screen_location.top_left;
        ei_size_t toplevel_size = toplevel->widget.screen_location.size;

        //Calcule des rectangles interieur au toplevel (titre + rectangle sous le titre)

        //titre
        ei_point_t text_spot = {toplevel_spot.x + border_width, toplevel_spot.y + border_width};
        ei_size_t text_size;
        hw_text_compute_size(*(title), ei_default_font, &(text_size.width), &(text_size.height));
        ei_rect_t title_rect = {text_spot, text_size};

        //Rectangle interieur sous le titre
        ei_rect_t inter;
        inter.top_left.x = text_spot.x + text_size.width + border_width;
        inter.top_left.y = text_spot.y;
        inter.size.width = toplevel_size.width - 2 * border_width;
        inter.size.height = toplevel_size.height - 2 * border_width - text_size.height;

        //Création du polygone exterieur en arrondissant le haut
        ei_linked_point_t* exter_first_point = points_list(widget->screen_location);
        /*
        Mettre un round frame ici
        */


        //Création du polygone du titre
        ei_linked_point_t* title_first_point = points_list(title_rect);

        //Création du polygone interieur (sous le titre)
        ei_linked_point_t* inter_first_point = points_list(inter);





        //Libération des polygones


}

void ei_toplevel_setdefaultsfunc (struct ei_widget_t* widget){
        ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
        toplevel->widget = *widget;
        toplevel->requested_size = widget->requested_size;
        toplevel->color = widget->pick_color;
        toplevel->border_width = 4;
        toplevel->title = "Toplevel";
        toplevel->closable = EI_TRUE;
        toplevel->resizable = ei_axis_both;
        toplevel->min_size.width = 160;
        toplevel->min_size.height = 120;
}

void ei_toplevel_geomnotifyfunc (struct ei_widget_t* widget, ei_rect_t rect){

}
