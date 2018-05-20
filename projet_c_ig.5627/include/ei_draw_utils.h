#include "ei_draw.h"
#include "ei_widget.h"
#include <stdio.h>

/**
        * calcul une nouvelle couleur à partir d'un couleur initiale
        *
        *@parameter initiale couleur que l'on veut modifier
        *
        *
        *@parameter res couleur obtenu
        *
        *
        *@ variation souahité
        *
        *
*/
void ei_compute_color(ei_color_t initiale, ei_color_t* res, float variation);

void ei_anchor_spot(ei_anchor_t anchor, ei_widget_t* widget, ei_point_t* anchor_position);

void ei_intersection_rectangle(ei_rect_t* first_rect, ei_rect_t* sec_rect, ei_rect_t* dest);   
