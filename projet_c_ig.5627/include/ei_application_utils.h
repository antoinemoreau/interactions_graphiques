#ifndef EI_APP_UTILS_H
#define EI_APP_UTILS_H

#include "ei_widget.h"
#include <stdlib.h>

/**
 * Parcours en largeur récursif affichant tous les widgets
 */
void            draw_all_widgets        (ei_widget_t*   widget,
                                         ei_surface_t   root_surface,
                                         ei_surface_t   pick_surface,
                                         ei_rect_t*     content_rect);

#endif
