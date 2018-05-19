#include "ei_placer.h"
#include <stdlib.h>
#include <stdio.h>

void compute_spot(ei_widget_t* widget, ei_widget_t* parent){
        ei_placer_t* placer_parent = (ei_placer_t*) parent->geom_params;
        ei_placer_t* placer_widget = (ei_placer_t*) widget->geom_params;

        if (placer_parent == NULL){
                /*
                Si on est sur le widget root, on calcule simplement la position du widget fils
                */
                fprintf(stdout, "coucou : %s\n", widget->screen_location);
                widget->screen_location.top_left.x = parent->screen_location.top_left.x + placer_widget->rel_x * placer_parent->width + placer_widget->x;
                widget->screen_location.top_left.y = parent->screen_location.top_left.y + placer_widget->rel_y * placer_parent->height + placer_widget->y;

        }
        else{   /*On calcule la place d'un des pixels du widget dans le référentiel du placer_parent
                  Le pixel est est déterminé par anchor (qui est donné par l'utilisateur)
                */
                widget->screen_location.top_left.x = placer_parent->x + placer_widget->rel_x * placer_parent->width + placer_widget->x;
                widget->screen_location.top_left.y = placer_parent->y + placer_widget->rel_y * placer_parent->height + placer_widget->y;
        }
        /*On alloue la bonne taille pour la fenêtre
        */
        widget->screen_location.size.width = placer_widget->width;
        widget->screen_location.size.height = placer_widget->height;
}


void ei_placer_runfunc(ei_widget_t* widget){
        /*
        On trouve le parent et on calcule la position de chacun de ses enfants
        */
        ei_widget_t* parent = widget->parent;

        for (ei_widget_t* current = parent->children_head; current != NULL; current=current->next_sibling){
                compute_spot(current, parent);
        }
}

void ei_placer_releasefunc(struct ei_widget_t* widget){

}
