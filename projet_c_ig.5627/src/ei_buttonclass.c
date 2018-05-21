#include "ei_buttonclass.h"
#include "ei_types.h"
#include "ei_button.h"
#include "ei_draw_utils.h"
#include <stdlib.h>
#include <stdio.h>
void*           ei_button_allocfunc             () {
        return calloc(1, sizeof(ei_button_t));
}

void            ei_button_releasefunc           (ei_widget_t* widget) {
        free(widget);
}

void            ei_button_drawfunc              (ei_widget_t*           widget,
                                                ei_surface_t		surface,
                                                ei_surface_t		pick_surface,
                                                ei_rect_t*		clipper) {
        ei_button_t* button = (ei_button_t*) widget;
        ei_rect_t unter = {clipper->top_left, clipper->size};
        //On récupère les 3 parties à colorier
        int nb_points = 8;
        ei_linked_point_t* low_part = rounded_frame(*clipper, button->corner_radius, nb_points, 0);
        ei_linked_point_t* high_part = rounded_frame(*clipper, button->corner_radius, nb_points, 1);
        ei_linked_point_t* all_part = rounded_frame(*clipper, button->corner_radius, nb_points, 2);

        ei_color_t light_color;
        ei_color_t dark_color;
        ei_compute_color(*button->color,&light_color,1.2);
        ei_compute_color(*frame->color,&dark_color,0.5);

        
        if (button->relief == ei_relief_raised) {
                // on draw les parties haute, basse, et l'intérieur du bouton

        }else if(button->relief == ei_relief_sunken){
                // on draw les parties haute, basse, et l'intérieur du bouton avec couleurs inversees
        }

        if (pick_surface) {
                /*
                hw_surface_lock(pick_surface);
                ei_fill(pick_surface,button->widget.pick_color,clipper);
                hw_surface_unlock(pick_surface);
                */
        }
        if (button->text) {
                /*
                ei_point_t aqui;
                ei_anchor_spot(button->text_anchor,&(button->widget),&aqui);
                ei_draw_text(surface,&aqui,*(button->text),NULL, *(button->text_color),clipper);
                */
        }

        //gestion des enfants
        ei_widget_t* current_child = button->widget.children_head;
        while (current_child){
                current_child->wclass->drawfunc(current_child, surface, pick_surface, current_child->content_rect);
                current_child = current_child->next_sibling;
        }
}

void            ei_button_setdefaultsfunc        (ei_widget_t* widget) {
        ei_button_t* button = (ei_button_t*) widget;
        button->widget = *widget;
        button->color = &ei_default_background_color;
        button->border_width = k_default_button_border_width;
        button->corner_radius = k_default_button_corner_radius;
        button->relief = ei_relief_raised;
        button->text = NULL;
        button->text_font = ei_default_font;
        button->text_color = &ei_font_default_color;
        button->text_anchor = ei_anc_center;
        button->img = NULL;
        button->img_rect = NULL;
        button->img_anchor = ei_anc_center;
        button->callback = NULL;
        button->user_param = NULL;
}

void            ei_button_geomnotifyfunc        (ei_widget_t* widget, ei_rect_t rect) {

}

ei_linked_point_t* rounded_frame(ei_rect_t rectangle, int rayon, int nb_points, int partie){
        /*
        Renvoie une chaine de points qui forme un rectangle avec des sommets arrondis
        On a ajouté un parametre nb_points qui permet de jouer sur la precision de l'arrondi

        si partie = 0 -> on arrondi seulement les sommets du bas
        si partie = 1 -> on arrondi seulement les sommets du haut
        si partie = 2 -> on arrondi tous les sommets
        */
        if (partie != 0 && partie != 1 && partie != 2){
                fprintf(stderr, "La partie sélectionnée n'existe pas : \n --> 0 = partie basse \n --> 1 = partie haute \n --> 2 = totalité \n" );
                exit(1);
        }
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
        ei_extreme_linked_points_t* extreme_points_top_right_low = arc(center_top_right, rayon, 0, 45, nb_points);
        ei_extreme_linked_points_t* extreme_points_top_right_high = arc(center_top_right, rayon, 45, 90, nb_points);
        ei_extreme_linked_points_t* extreme_points_top_left = arc(center_top_left, rayon, 90, 180, nb_points);
        ei_extreme_linked_points_t* extreme_points_bot_left_high = arc(center_bot_left, rayon, 180, 225, nb_points);
        ei_extreme_linked_points_t* extreme_points_bot_left_low = arc(center_bot_left, rayon, 225, 270, nb_points);
        ei_extreme_linked_points_t* extreme_points_bot_right = arc(center_bot_right, rayon, 270, 360, nb_points);

        //On déclare le premier point de la liste qu'on renvoie
        ei_linked_point_t* first_point = calloc(1, sizeof(ei_linked_point_t));

        //On relie tous les points dans l'ordre selon la partie sélectionnée
        if (partie == 2){
                extreme_points_top_left->tail_point->next = extreme_points_bot_left_high->head_point;
                extreme_points_bot_left_high->tail_point->next = extreme_points_bot_left_low->head_point;
                extreme_points_bot_left_low->tail_point->next = extreme_points_bot_right->head_point;
                extreme_points_bot_right->tail_point->next = extreme_points_top_right_low->head_point;
                extreme_points_bot_left_low->tail_point->next = extreme_points_top_right_high->head_point;
                first_point = extreme_points_top_left->head_point;
        }
        else if (partie == 0){
                extreme_points_bot_left_low->tail_point->next = extreme_points_bot_right->head_point;
                extreme_points_bot_right->tail_point->next = extreme_points_top_right_low->head_point;
                first_point = extreme_points_bot_left_low->head_point;
        }
        else{
                extreme_points_bot_left_high->tail_point->next = extreme_points_top_left->head_point;
                extreme_points_top_left->tail_point->next = extreme_points_bot_left_high->head_point;
                first_point = extreme_points_top_right_high->head_point;
        }
        free(extreme_points_top_left);
        free(extreme_points_top_right_low);
        free(extreme_points_top_right_high);
        free(extreme_points_bot_left_low);
        free(extreme_points_bot_left_high);
        free(extreme_points_bot_right);
        //On retourne alors le premier point de la chaine formée
        return first_point;
}
