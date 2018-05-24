#ifndef EI_TOPLEVELCLASS_H
#define EI_TOPLEVELCLASS_H
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"

void* ei_toplevel_allocfunc ();

void ei_toplevel_releasefunc (struct ei_widget_t* widget);

void ei_toplevel_drawfunc (struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper);

void ei_toplevel_setdefaultsfunc (struct ei_widget_t* widget);

void ei_toplevel_geomnotifyfunc (struct ei_widget_t* widget, ei_rect_t rect);

#endif
