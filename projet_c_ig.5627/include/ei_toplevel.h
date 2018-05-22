#include "ei_widget.h"
#include "ei_widgetclass.h"


typedef struct ei_toplevel_t {
	ei_widget_t		widget;
	ei_size_t	        requested_size;
        ei_color_t*             color;
        int                     border_width;
        char**                  title;
        ei_bool_t              	closable;
        ei_axis_set_t          	resizable;
        ei_size_t             	min_size;
} ei_toplevel_t;
