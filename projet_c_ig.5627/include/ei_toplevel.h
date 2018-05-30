#ifndef EI_TOPLEVEL_H
#define EI_TOPLEVEL_H
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_button.h"

/**
 * @brief       A structure which describes a toplevel widget.
 * 
 */
typedef struct ei_toplevel_t {
	ei_widget_t		widget;
	ei_button_t*		close_button;
        ei_color_t              color;
        int                     border_width;
        char*                   title;
        ei_bool_t              	closable;
        ei_axis_set_t          	resizable;
        ei_size_t*             	min_size;
} ei_toplevel_t;

#endif
