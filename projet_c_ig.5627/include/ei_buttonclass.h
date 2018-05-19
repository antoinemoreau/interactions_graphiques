#ifndef EI_BUTTONCLASS_H
#define EI_BUTTONCLASS_H

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_button.h"

void* ei_button_allocfunc ();

void ei_button_releasefunc (struct ei_widget_t* button);

void ei_button_drawfunc (struct ei_widget_t* button, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper);

void ei_button_setdefaultsfunc (struct ei_widget_t* button);

void ei_button_geomnotifyfunc (struct ei_widget_t* button, ei_rect_t rect);

#endif
