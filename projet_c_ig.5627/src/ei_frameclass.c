#include "ei_frameclass.h"
#include "ei_types.h"
#include "ei_frame.h"
#include <stdlib.h>

void* ei_frame_allocfunc () {
    return calloc(sizeof(ei_frame_t), 1);
}

void ei_frame_releasefunc (struct ei_frame_t* frame) {

}

void ei_frame_drawfunc (struct ei_frame_t*	frame,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {

}

void ei_frame_setdefaultsfunc (struct ei_frame_t* frame) {
    frame->widget = (ei_widget_t*)frame;
    (frame->requested_size)->width = 0;
    (frame->requested_size)->height = 0;
    *(frame->color) = ei_default_background_color;
    frame->border_width = 0;
    frame->relief = ei_relief_none;
    frame->text = NULL;
    frame->text_font = ei_default_font;
    *(frame->text_color) = ei_font_default_color;
    *(frame->text_anchor) = ei_anc_center;
    frame->img = NULL;
    frame->img_rect = NULL;
    *(frame->img_anchor) = ei_anc_center;

}

void ei_frame_geomnotifyfunc (struct ei_frame_t* frame, ei_rect_t rect) {

}
