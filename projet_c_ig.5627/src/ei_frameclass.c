#include "ei_frameclass.h"
#include "ei_types.h"

void* ei_frameclass_allocfunc_t () {
    return calloc(sizeof(ei_frame_t), 1);
}

void ei_frameclass_releasefunc_t (struct ei_frame_t* frame) {

}

void ei_frameclass_drawfunc_t (struct ei_frame_t*	frame,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {

}

void ei_frameclass_setdefaultsfunc_t (struct ei_frame_t* frame) {
    frame->widget = frame_widget;
    frame->requested_size = (0, 0);
    frame->color = ei_default_backgroung_color;
    frame->border_width = 0;
    frame->relief = ei_relief_none;
    frame->text = NULL;
    frame->text_font = ei_default_font;
    frame->text_color = ei_font_default_color;
    frame->text_anchor = ei_anc_center;
    frame->img = NULL;
    frame->img_rect = NULL;
    frame->img_anchor = ei_anc_center;
}

void ei_frameclass_geomnotifyfunc_t (struct ei_frame_t* frame, ei_rect_t rect) {

}
