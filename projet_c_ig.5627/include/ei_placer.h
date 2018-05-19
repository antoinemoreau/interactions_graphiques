#ifndef EI_PLACER_H
#define EI_PLACER_H

#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"

typedef struct ei_placer_t {
	ei_geometrymanager_t*         manager;
        ei_anchor_t                   anchor;
        int                           x;
        int                           y;
        int                           width;
        int                           height;
        float                         rel_x;
        float                         rel_y;
        float                         rel_width;
        float                         rel_height;
} ei_placer_t;

void ei_placer_runfunc(struct ei_widget_t* widget);

void ei_placer_releasefunc(struct ei_widget_t* widget);

#endif
