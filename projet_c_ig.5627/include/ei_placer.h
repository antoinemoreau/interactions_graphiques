#ifndef EI_PLACER_H
#define EI_PLACER_H

#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"

void ei_placer_runfunc(struct ei_widget_t* widget);

void ei_placer_releasefunc(struct ei_widget_t* widget);

#endif
