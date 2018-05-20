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

typedef struct {
	ei_linked_point_t*			head_point;
	ei_linked_point_t*			tail_point;
} ei_extreme_linked_points_t;

void ei_compute_color(ei_color_t initiale, ei_color_t* res, float variation);

void ei_anchor_spot(ei_anchor_t anchor, ei_widget_t* widget, ei_point_t* anchor_position);

void ei_intersection_rectangle(ei_rect_t* first_rect, ei_rect_t* sec_rect, ei_rect_t* dest);

ei_extreme_linked_points_t* arc(ei_point_t center, int rayon, int angle_depart, int angle_fin, int nb_points);

static int coordonnee_minimum(int x1, int x2);

static int coordonnee_maximum(int x1, int x2);

static ei_rect_t* rectangle_abs_min(ei_rect_t* clipper, ei_rect_t* sec_rect);

static ei_rect_t* rectangle_ord_min(ei_rect_t* clipper, ei_rect_t* sec_rect);
