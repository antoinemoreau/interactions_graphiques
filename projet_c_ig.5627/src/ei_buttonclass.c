#include "ei_buttonclass.h"
#include "ei_types.h"
#include "ei_button.h"
#include <stdlib.h>

void*           ei_button_allocfunc             () {
        return calloc(1, sizeof(ei_button_t));
}

void            ei_button_releasefunc           (ei_widget_t* widget) {

}

void            ei_button_drawfunc              (ei_widget_t*           widget,
                                                ei_surface_t		surface,
                                                ei_surface_t		pick_surface,
                                                ei_rect_t*		clipper) {

}

void            ei_button_setdefaultsfunc        (ei_widget_t* widget) {

}

void            ei_button_geomnotifyfunc        (ei_widget_t* widget, ei_rect_t rect) {

}