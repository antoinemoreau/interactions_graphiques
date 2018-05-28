#include "ei_placer.h"
#include <stdlib.h>
#include <stdio.h>

static void compute_spot(ei_widget_t* widget, ei_widget_t* parent, int x, int y) {
        ei_placer_t* placer_widget = (ei_placer_t*) widget->geom_params;
        if (parent != NULL) {
                ei_placer_t* placer_parent = (ei_placer_t*) parent->geom_params;

                if (placer_parent == NULL) {
                        /*
                        Si le parent est le widget root, on calcule simplement la position du widget
                        */
                        widget->screen_location.top_left.x = placer_widget->rel_x * parent->screen_location.size.width + placer_widget->x - x;
                        widget->screen_location.top_left.y = placer_widget->rel_y * parent->screen_location.size.height + placer_widget->y - y;

                } else {   /*On calcule la place d'un des pixels du widget dans le référentiel du placer_parent
                          Le pixel est est déterminé par anchor (qui est donné par l'utilisateur)
                        */
                        widget->screen_location.top_left.x = parent->content_rect->top_left.x + placer_widget->rel_x * placer_parent->width + placer_widget->x - x;
                        widget->screen_location.top_left.y = parent->content_rect->top_left.y + placer_widget->rel_y * placer_parent->height + placer_widget->y - y;
                        printf("screen location top left: %d \n", widget->screen_location.top_left.y);
                }
        }
}

static void compute_size(ei_widget_t* widget, ei_widget_t* parent){
        ei_placer_t* placer_widget = (ei_placer_t*) widget->geom_params;
        if (parent != NULL) {
                widget->screen_location.size.width = placer_widget->rel_width * parent->content_rect->size.width + placer_widget->width;
                widget->screen_location.size.height = placer_widget->rel_height * parent->content_rect->size.height + placer_widget->height;
        }
}


void ei_placer_runfunc(ei_widget_t* widget){
        /*
        On trouve le parent et on calcule la position de son enfant
        */
        ei_widget_t* parent = widget->parent;

        ei_placer_t* placer = (ei_placer_t*) widget->geom_params;
        compute_size(widget, parent);
        int x, y;
        ei_anchor_t anchor = placer->anchor;
        printf("anchor %d \n", anchor);
        switch (anchor) {
                case ei_anc_center:
                        x = widget->screen_location.size.width/2;
                        y = widget->screen_location.size.height/2;
                        break;
                case ei_anc_north:
                        x = widget->screen_location.size.width/2;
                        y = 0;
                        break;
                case ei_anc_northeast:
                        x = widget->screen_location.size.width;
                        y = 0;
                        break;
                case ei_anc_east:
                        x = widget->screen_location.size.width;
                        y = widget->screen_location.size.height/2;
                        break;
                case ei_anc_southeast:
                        x = widget->screen_location.size.width;
                        y = widget->screen_location.size.height;
                        printf("mdr l'anchor x : %d y : %d \n", x, y);

                        break;
                case ei_anc_south:
                        x = widget->screen_location.size.width/2;
                        y = widget->screen_location.size.height;
                        break;
                case ei_anc_southwest:
                        x = 0;
                        y = widget->screen_location.size.height;
                        break;
                case ei_anc_west:
                        x = 0;
                        y = widget->screen_location.size.height/2;
                        break;
                default:
                        x = 0;
                        y = 0;
        }
        compute_spot(widget, parent, x, y);
}

static void supress_widget_from_parent(ei_widget_t* widget){
        /*
        Fonction pour faciliter la supression d'un widget dans la
        liste des enfants de son placer_parent (appelée dans release)

        Condition : la parent existe et le widget est dans la liste
        */
        ei_widget_t* parent = widget->parent;
        ei_widget_t* current = parent->children_head;
        ei_widget_t* previous = parent->children_head;
        //On cherche le widget passé en argument et on enlève les pointeurs sur lui
        while (current != NULL) {
                if (current == widget){
                        previous->next_sibling = current->next_sibling;
                        if (current == parent->children_tail){
                                parent->children_tail = previous;
                        }
                        return;
                }
                previous = current;
                current = current->next_sibling;
        }
}

void ei_placer_releasefunc(struct ei_widget_t* widget){
        ei_widget_t* parent = widget->parent;
        if (parent != NULL){
                /*
                On enlève les liens entre ce widget et ses enfants et son parent
                */
                for (ei_widget_t* current = widget->children_head; current != NULL; current = current->next_sibling){
                        if (parent->children_tail != NULL){
                                parent->children_tail->next_sibling = current;
                                parent->children_tail = current;
                        }
                        else{
                                parent->children_head = current;
                                parent->children_tail = current;
                                current->next_sibling = NULL;
                        }
                }
                supress_widget_from_parent(widget);
        }
        else{
                for (ei_widget_t* current = widget->children_head; current != NULL; current = current->next_sibling){
                        current->parent = NULL;
                }
        }
}
