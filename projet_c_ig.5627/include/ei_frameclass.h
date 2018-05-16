#ifndef EI_FRAMECLASS_H
#define EI_FRAMECLASS_H

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_frame.h"

void* ei_frameclass_allocfunc ();

void ei_frameclass_releasefunc (struct ei_frame_t* frame);

void ei_frameclass_drawfunc (struct ei_frame_t*	frame,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);

void ei_frameclass_setdefaultsfunc (struct ei_frame_t* frame);

void ei_frameclass_geomnotifyfunc (struct ei_frame_t* frame, ei_rect_t rect);

ei_widgetclass_t frame_widget = {
    frame_classname,
    &ei_frameclass_allocfunc,
    &ei_frameclass_releasefunc,
    &ei_frameclass_drawfunc,
    &ei_frameclass_setdefaultsfunc,
    &ei_frameclass_geomnotifyfunc,
    NULL
};

#endif
