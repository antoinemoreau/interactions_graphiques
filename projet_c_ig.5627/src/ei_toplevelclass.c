#include "ei_toplevelclass.h"
#include "ei_toplevel.h"
#include "ei_draw_utils.h"
#include "ei_types.h"
#include "ei_button.h"
#include "ei_buttonclass.h"
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

static ei_button_t* closing_button(ei_toplevel_t* toplevel) {
        //Récupération de la border_width du toplevel
        int             toplevel_border_width   = toplevel->border_width;


        //Définition des paramètres du bouton
        int             radius                  = 10;
        int             diameter                = 2 * radius;

        int             button_border_width     = 2;
        ei_color_t      button_color            = {0xa9,0x11,0x01, 0xff};
        ei_anchor_t	button_anchor	        = ei_anc_northwest;
        float           button_rel_x            = 0.0;
        float           button_rel_y            = 0.0;
        int		button_x	        = toplevel_border_width;
        int		button_y	        = toplevel_border_width;
        ei_relief_t     relief                  = ei_relief_raised;
        ei_size_t       requested_size          = {diameter, diameter}; // On les défini en dur mais faut changer

        //Création et configuration du bouton suivant les paramètres
        ei_widget_t*    button_widget           = ei_widget_create ("button", NULL);
        button_widget->parent = (ei_widget_t*) toplevel;
        ei_button_t*    button                  = (ei_button_t*) button_widget;

        ei_button_configure(button_widget, &requested_size, &button_color,
                            &button_border_width, &radius, &relief, NULL, NULL, NULL, NULL,
                            NULL, NULL, NULL, NULL, NULL);


        //Création d'un bouton avant le texte pour fermer la fenêtre
        ei_place(button_widget, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL);        

        return button;
}




void* ei_toplevel_allocfunc () {
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


        ei_toplevel_t*          toplevel              = (ei_toplevel_t*) widget;
        ei_axis_set_t           resizable             = toplevel->resizable;
        ei_bool_t               closable              = toplevel->closable;

        ei_color_t              color                 = *(toplevel->color);
        int                     border_width          = toplevel->border_width;

        ei_point_t              toplevel_spot         = toplevel->widget.screen_location.top_left;
        ei_size_t               frame_size            = toplevel->widget.content_rect->size;


        //titre
        ei_point_t text_spot = {toplevel_spot.x + border_width + 25, toplevel_spot.y + border_width};
        ei_size_t text_size;
        hw_text_compute_size(*(toplevel->title), ei_default_font, &(text_size.width), &(text_size.height));

        //Calcule de la position du rectangle sous le titre et de la taille du toplevel
        ei_point_t frame_spot = {toplevel_spot.x + border_width, toplevel_spot.y + 2 * border_width + text_size.height};
        ei_size_t toplevel_size = {frame_size.width + 2 * border_width, frame_size.height + text_size.height + 3 * border_width};

        //Clipping de la toplevel en fonction du parent
        toplevel->widget.screen_location.size = toplevel_size;
        ei_rect_t intersection = {toplevel->widget.screen_location.top_left,toplevel->widget.screen_location.size};
        ei_intersection_rectangle(clipper, &toplevel->widget.screen_location, &intersection);

        //Calcule du clipper du rectangle sous le titre
        ei_rect_t frame_rect = {frame_spot, frame_size};
        ei_rect_t interieur = frame_rect;
        ei_intersection_rectangle(clipper, &frame_rect, &interieur); // Peut etre changer clipper avec intersection

        //Calcule du clipper de la toplevel
        toplevel->widget.content_rect->top_left = interieur.top_left;
        toplevel->widget.content_rect->size = interieur.size;

        //Création du polygone exterieur en arrondissant le haut
        int nb_points = 10;
        int rayon = 20;
        ei_point_t center_top_left = {toplevel_spot.x + rayon, toplevel_spot.y + rayon};
        ei_point_t center_top_right = {toplevel_spot.x - rayon + toplevel_size.width, toplevel_spot.y + rayon};
        ei_extreme_linked_points_t* arc_top_left = arc(center_top_left, rayon, 90.0, 180.0, nb_points);
        ei_extreme_linked_points_t* arc_top_right = arc(center_top_right, rayon, 0.0, 90.0, nb_points);

        ei_point_t point_bot_left = {toplevel_spot.x, toplevel_spot.y + toplevel_size.height};
        ei_point_t point_bot_right = {toplevel_spot.x + toplevel_size.width, toplevel_spot.y + toplevel_size.height};

        ei_linked_point_t* bot_right = calloc(1, sizeof(ei_linked_point_t));
        bot_right->point = point_bot_right;

        ei_linked_point_t* bot_left = calloc(1, sizeof(ei_linked_point_t));
        bot_left->point = point_bot_left;
        bot_left->next = bot_right;
        ei_linked_point_t* exter_first_point = arc_top_right->head_point;
        arc_top_right->tail_point->next = arc_top_left->head_point;
        arc_top_left->tail_point->next = bot_left;

        //Chainage : arc_top_right->arc_top_left->bot_left->bot_right
        ei_draw_polygon(surface, exter_first_point, color, &intersection);

        ei_color_t inter_color = {0xff, 0xff, 0xff, 0xff};

        //Création du polygone interieur (sous le titre)
        ei_linked_point_t* inter_first_point = points_list(frame_rect);
        ei_draw_polygon(surface, inter_first_point, inter_color, &interieur);

        if (resizable != ei_axis_none){
                //Création du polygone pour resize
                int square_size = 2 * border_width;
                ei_rect_t resize_rect;
                resize_rect.top_left.x = toplevel_spot.x + toplevel_size.width - square_size;
                resize_rect.top_left.y = toplevel_spot.y + toplevel_size.height - square_size;
                resize_rect.size.width = square_size;
                resize_rect.size.height = square_size;

                ei_draw_polygon(surface, points_list(resize_rect), color, &intersection);
        }

        if (closable == EI_TRUE){
                //Dessin du bouton en haut à gauche du
                ei_widget_t* button_widget = (ei_widget_t*) toplevel->close_button;


                ei_button_drawfunc(button_widget, surface, pick_surface, clipper);
        }


        if (pick_surface) {

                // hw_surface_lock(pick_surface);
                // ei_fill(pick_surface,button->widget.pick_color,clipper);
                // hw_surface_unlock(pick_surface);

        }

        if (toplevel->title && strcmp(*(toplevel->title),"") != 0) {
                ei_rect_t title_rect = {text_spot, text_size};

                ei_point_t aqui;
                ei_anchor_spot(ei_anc_none, &text_size,&title_rect,&aqui);
                ei_draw_text(surface,&aqui,*(toplevel->title),ei_default_font, ei_font_default_color, clipper);

        }
        //Libération des polygones

        free(exter_first_point);
        free(inter_first_point);
}

void ei_toplevel_setdefaultsfunc (struct ei_widget_t* widget){
        ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
        toplevel->widget = *widget;
        toplevel->color = widget->pick_color;
        toplevel->border_width = 4;
        *(toplevel->title) = "Toplevel";
        toplevel->closable = EI_TRUE;
        toplevel->resizable = ei_axis_both;
        toplevel->min_size = calloc(1, sizeof(ei_size_t));
        toplevel->min_size->width = 160;
        toplevel->min_size->height = 120;
        toplevel->close_button = closing_button(toplevel);
}

void ei_toplevel_geomnotifyfunc (struct ei_widget_t* widget, ei_rect_t rect){

}
