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