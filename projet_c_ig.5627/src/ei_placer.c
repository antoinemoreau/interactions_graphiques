#include "ei_placer.h"
#include <stdlib.h>
#include <stdio.h>

static void compute_spot(ei_widget_t* widget, ei_widget_t* parent, int x, int y){
        ei_placer_t* placer_parent = (ei_placer_t*) parent->geom_params;
        ei_placer_t* placer_widget = (ei_placer_t*) widget->geom_params;

        if (placer_parent == NULL){
                /*
                Si on est sur le widget root, on calcule simplement la position du widget fils
                */
                widget->screen_location.top_left.x = parent->screen_location.top_left.x + placer_widget->rel_x * parent->screen_location.size.width + placer_widget->x - x;
                widget->screen_location.top_left.y = parent->screen_location.top_left.y + placer_widget->rel_y * parent->screen_location.size.height + placer_widget->y - y;

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
                ei_placer_t* placer = (ei_placer_t*) widget->geom_params;
                int x, y;
                ei_anchor_t anchor = placer->anchor;
                switch (anchor) {
                        case ei_anc_center:
                                x = widget->content_rect->size.width/2;
                                y = widget->content_rect->size.height/2;
                                break;
                        case ei_anc_north:
                                x = widget->content_rect->size.width/2;
                                y = 0;
                                break;
                        case ei_anc_northeast:
                                x = widget->content_rect->size.width;
                                y = 0;
                                break;
                        case ei_anc_east:
                                x = widget->content_rect->size.width;
                                y = widget->content_rect->size.height/2;
                                break;
                        case ei_anc_southeast:
                                x = widget->content_rect->size.width;
                                y = widget->content_rect->size.height;
                                break;
                        case ei_anc_south:
                                x = widget->content_rect->size.width/2;
                                y = widget->content_rect->size.height;
                                break;
                        case ei_anc_southwest:
                                x = 0;
                                y = widget->content_rect->size.height;
                                break;
                        case ei_anc_west:
                                x = 0;
                                y = widget->content_rect->size.height/2;
                                break;
                        default:
                                x = 0;
                                y = 0;
                }
                compute_spot(current, parent, x, y);
        }
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
