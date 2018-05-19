#include "ei_placer.h"
#include <stdlib.h>
#include <stdio.h>

void compute_spot(ei_widget_t* widget, ei_widget_t* parent){
        ei_placer_t* placer_parent = (ei_placer_t*) parent->geom_params;
        ei_placer_t* placer_widget = (ei_placer_t*) widget->geom_params;

        if (placer_parent == NULL){
                widget->screen_location.top_left.x = parent->screen_location.top_left.x + placer_widget->x;
                widget->screen_location.top_left.y = parent->screen_location.top_left.y + placer_widget->y;

        }
        else{
                widget->screen_location.top_left.x = placer_parent->x + placer_widget->x;
                widget->screen_location.top_left.y = placer_parent->y + placer_widget->y;
                widget->screen_location.size = widget->requested_size;
        }
}


void ei_placer_runfunc(ei_widget_t* widget){
        ei_widget_t* parent = widget->parent;
        for (ei_widget_t* current = parent->children_head; current != NULL; current=current->next_sibling){
                compute_spot(current, parent);
        }
}

void ei_placer_releasefunc(struct ei_widget_t* widget){

}
