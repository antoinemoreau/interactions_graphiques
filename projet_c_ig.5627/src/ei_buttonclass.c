#include "ei_buttonclass.h"
#include "ei_types.h"
#include "ei_button.h"
#include <stdlib.h>

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