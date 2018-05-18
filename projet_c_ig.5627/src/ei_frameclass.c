#include "ei_frameclass.h"
#include "ei_types.h"
#include "ei_frame.h"
#include <stdlib.h>
#include <stdio.h>

void* ei_frame_allocfunc () {
        return calloc(1, sizeof(ei_frame_t));
}


void ei_frame_releasefunc (struct ei_widget_t* widget) {
        free(widget);
}

void ei_frame_drawfunc      (ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper) {

        ei_frame_t* frame = (ei_frame_t*) widget;
        ei_fill(surface,frame->color,clipper);
        //ei_fill(pick_surface,frame->widget->pick_color,clipper);
}

void ei_frame_setdefaultsfunc (ei_widget_t* widget) {
        //fprintf(stdout, "coucou : %s\n", widget->wclass->name);
        ei_frame_t* frame = (ei_frame_t*) widget;

        frame->widget = *widget;

        frame->color = &ei_default_background_color;
        frame->border_width = 0;
        frame->relief = ei_relief_none;
        frame->text = NULL;
        frame->text_font = ei_default_font;
        frame->text_color = &ei_font_default_color;
        frame->text_anchor = ei_anc_center;
        frame->img = NULL;
        frame->img_rect = NULL;
        frame->img_anchor = ei_anc_center;

}

void ei_frame_geomnotifyfunc (struct ei_widget_t* widget, ei_rect_t rect) {

}
