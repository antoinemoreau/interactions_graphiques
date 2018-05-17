#ifndef EI_FRAMECLASS_H
#define EI_FRAMECLASS_H

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_frame.h"

void* ei_frame_allocfunc ();

void ei_frame_releasefunc (struct ei_frame_t* frame);

void ei_frame_drawfunc (struct ei_frame_t*	frame,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);

void ei_frame_setdefaultsfunc (struct ei_frame_t* frame);

void ei_frame_geomnotifyfunc (struct ei_frame_t* frame, ei_rect_t rect);

#endif
