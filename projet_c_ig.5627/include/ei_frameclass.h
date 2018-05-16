#ifndef EI_FRAMECLASS_H
#define EI_FRAMECLASS_H

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_frame.h"

struct ei_frameclass_t frame;

frame.allocfunc = &ei_frameclass_allocfunc_t;
frame.releasefunc = &ei_frameclass_releasefunc_t;
frame.drawfunc = &ei_frameclass_drawfunc_t;
frame.setdefaultsfunc = &ei_frameclass_setdefaultsfunc_t;
frame.geomnotifyfunc = &ei_widgetclass_geomnotifyfunc_t;

void* ei_frameclass_allocfunc_t ();

void ei_frameclass_releasefunc_t (struct ei_frame_t* frame);

void ei_frameclass_drawfunc_t (struct ei_frame_t*	frame,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);

void ei_frameclass_setdefaultsfunc_t (struct ei_frame_t* frame);

void ei_frameclass_geomnotifyfunc_t (struct ei_frame_t* frame, ei_rect_t rect);

#endif
