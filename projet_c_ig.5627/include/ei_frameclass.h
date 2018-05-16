#ifndef EI_FRAMECLASS_H
#define EI_FRAMECLASS_H

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_frame.h"

#define FRAME_CLASS

struct ei_widgetclass_t frame_widget = {
    FRAME_CLASS,
    &ei_frameclass_allocfunc_t,
    &ei_frameclass_releasefunc_t,
    &ei_frameclass_drawfunc_t,
    &ei_frameclass_setdefaultsfunc_t,
    &ei_widgetclass_geomnotifyfunc_t,
    NULL
}

void* ei_frameclass_allocfunc_t ();

void ei_frameclass_releasefunc_t (struct ei_frame_t* frame);

void ei_frameclass_drawfunc_t (struct ei_frame_t*	frame,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);

void ei_frameclass_setdefaultsfunc_t (struct ei_frame_t* frame);

void ei_frameclass_geomnotifyfunc_t (struct ei_frame_t* frame, ei_rect_t rect);

#endif
