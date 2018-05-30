#include "ei_placer.h"
#include <stdlib.h>
#include <stdio.h>

static void compute_spot(ei_widget_t *widget, ei_widget_t *parent, int x, int y)
{
        ei_placer_t *placer_widget = (ei_placer_t *)widget->geom_params;
        if (parent != NULL)
        {
                // Computing the position of a pixel in the given widget in the absolute coordinate system
                // The pixel is determined by the anchor parameter
                widget->screen_location.top_left.x = parent->content_rect->top_left.x + placer_widget->rel_x * parent->content_rect->size.width + placer_widget->x - x;
                widget->screen_location.top_left.y = parent->content_rect->top_left.y + placer_widget->rel_y * parent->content_rect->size.height + placer_widget->y - y;
                widget->wclass->geomnotifyfunc(widget, widget->screen_location);
        }
}

static void compute_size(ei_widget_t *widget, ei_widget_t *parent)
{
        ei_placer_t *placer_widget = (ei_placer_t *)widget->geom_params;
        if (parent != NULL)
        {
                widget->screen_location.size.width = placer_widget->rel_width * parent->screen_location.size.width + placer_widget->width;
                widget->screen_location.size.height = placer_widget->rel_height * parent->screen_location.size.height + placer_widget->height;
        }
}

void ei_placer_runfunc(ei_widget_t *widget)
{
       // Finding the parent and computing his children position
        ei_widget_t *parent = widget->parent;

        ei_placer_t *placer = (ei_placer_t *)widget->geom_params;
        compute_size(widget, parent);
        int x, y;
        ei_anchor_t anchor = placer->anchor;
        switch (anchor)
        {
        case ei_anc_center:
                x = widget->screen_location.size.width / 2;
                y = widget->screen_location.size.height / 2;
                break;
        case ei_anc_north:
                x = widget->screen_location.size.width / 2;
                y = 0;
                break;
        case ei_anc_northeast:
                x = widget->screen_location.size.width;
                y = 0;
                break;
        case ei_anc_east:
                x = widget->screen_location.size.width;
                y = widget->screen_location.size.height / 2;
                break;
        case ei_anc_southeast:
                x = widget->screen_location.size.width;
                y = widget->screen_location.size.height;
                break;
        case ei_anc_south:
                x = widget->screen_location.size.width / 2;
                y = widget->screen_location.size.height;
                break;
        case ei_anc_southwest:
                x = 0;
                y = widget->screen_location.size.height;
                break;
        case ei_anc_west:
                x = 0;
                y = widget->screen_location.size.height / 2;
                break;
        default:
                x = 0;
                y = 0;
        }
        compute_spot(widget, parent, x, y);

        for (ei_widget_t *child = widget->children_head; child != NULL; child = child->next_sibling)
        {
                if (child->geom_params != NULL)
                        child->geom_params->manager->runfunc(child);
        }
}

void ei_placer_releasefunc(struct ei_widget_t *widget)
{
        // No need for a release function in the placer
        return;
}
