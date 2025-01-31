#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_draw_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265

void ei_compute_color(ei_color_t initiale, ei_color_t *res, float variation)
{
        int rouge = (int)initiale.red * variation;
        int bleu = (int)initiale.blue * variation;
        int vert = (int)initiale.green * variation;
        if (rouge < 255)
        {
                res->red = rouge;
        }
        else
        {
                res->red = 255;
        }
        if (bleu < 255)
        {
                res->blue = bleu;
        }
        else
        {
                res->blue = 255;
        }
        if (vert < 255)
        {
                res->green = vert;
        }
        else
        {
                res->green = 255;
        }
        res->alpha = initiale.alpha;
}

static ei_rect_t *rectangle_abs_min(ei_rect_t *clipper, ei_rect_t *sec_rect)
{
        if (clipper->top_left.x <= sec_rect->top_left.x)
        {
                return clipper;
        }
        else
        {
                return sec_rect;
        }
}

static ei_rect_t *rectangle_ord_min(ei_rect_t *clipper, ei_rect_t *sec_rect)
{
        if (clipper->top_left.y <= sec_rect->top_left.y)
        {
                return clipper;
        }
        else
        {
                return sec_rect;
        }
}

void ei_intersection_rectangle(ei_rect_t *clipper, ei_rect_t *sec_rect, ei_rect_t *dest)
{
        ei_rect_t *left_rect = rectangle_abs_min(clipper, sec_rect);
        ei_rect_t *right_rect = NULL;
        ei_rect_t *top_rect = rectangle_ord_min(clipper, sec_rect);
        ei_rect_t *bot_rect = NULL;
        if (left_rect == clipper)
        {
                right_rect = sec_rect;
        }
        else
        {
                right_rect = clipper;
        }
        if (top_rect == clipper)
        {
                bot_rect = sec_rect;
        }
        else
        {
                bot_rect = clipper;
        }

        // Destination rectangle
        ei_point_t top_dest;
        ei_size_t dest_size;

        if ((left_rect->top_left.x + left_rect->size.width < right_rect->top_left.x) || top_rect->top_left.y + top_rect->size.height < bot_rect->top_left.y)
        {
                top_dest.x = 0;
                top_dest.y = 0;
                dest_size.width = 0;
                dest_size.height = 0;
        }
        else
        {
                int marge_abs, marge_ord;
                if (right_rect->top_left.x + right_rect->size.width <= left_rect->top_left.x + left_rect->size.width)
                {
                        marge_abs = left_rect->top_left.x + left_rect->size.width - (right_rect->top_left.x + right_rect->size.width);
                }
                else
                {
                        marge_abs = 0;
                }
                if (bot_rect->top_left.y + bot_rect->size.height <= top_rect->top_left.y + top_rect->size.height)
                {
                        marge_ord = top_rect->top_left.y + top_rect->size.height - (bot_rect->top_left.y + bot_rect->size.height);
                }
                else
                {
                        marge_ord = 0;
                }
                top_dest.x = right_rect->top_left.x;
                top_dest.y = bot_rect->top_left.y;
                dest_size.width = left_rect->top_left.x + left_rect->size.width - top_dest.x - marge_abs;
                dest_size.height = top_rect->top_left.y + top_rect->size.height - top_dest.y - marge_ord;
        }
        dest->top_left = top_dest;
        dest->size = dest_size;
}

void ei_anchor_spot(ei_anchor_t anchor, ei_size_t *size, ei_rect_t *rectangle, ei_point_t *anchor_position)
{
        switch (anchor)
        {
        case ei_anc_center:
                (*anchor_position).x = rectangle->top_left.x + rectangle->size.width / 2 - size->width / 2;
                (*anchor_position).y = rectangle->top_left.y + rectangle->size.height / 2 - size->height / 2;
                break;
        case ei_anc_north:
                (*anchor_position).x = rectangle->top_left.x + rectangle->size.width / 2 - size->width / 2;
                (*anchor_position).y = rectangle->top_left.y;
                break;
        case ei_anc_northeast:
                (*anchor_position).x = rectangle->top_left.x + rectangle->size.width - size->width;
                (*anchor_position).y = rectangle->top_left.y;
                break;
        case ei_anc_east:
                (*anchor_position).x = rectangle->top_left.x + rectangle->size.width - size->width;
                (*anchor_position).y = rectangle->top_left.y + rectangle->size.height / 2 - size->height / 2;
                break;
        case ei_anc_southeast:
                (*anchor_position).x = rectangle->top_left.x + rectangle->size.width - size->width;
                (*anchor_position).y = rectangle->top_left.y + rectangle->size.height - size->height;
                break;
        case ei_anc_south:
                (*anchor_position).y = rectangle->top_left.y + rectangle->size.height - size->height;
                (*anchor_position).x = rectangle->top_left.x + rectangle->size.width / 2 - size->width / 2;
                break;
        case ei_anc_southwest:
                (*anchor_position).y = rectangle->top_left.y + rectangle->size.height - size->height;
                (*anchor_position).x = rectangle->top_left.x;
                break;
        case ei_anc_west:
                (*anchor_position).x = rectangle->top_left.x;
                (*anchor_position).y = rectangle->top_left.y + rectangle->size.height / 2 - size->height;
                break;
        default:
                (*anchor_position).x = rectangle->top_left.x;
                (*anchor_position).y = rectangle->top_left.y;
        }
}

ei_extreme_linked_points_t *arc(ei_point_t center, int rayon, float angle_depart, float angle_fin, int nb_points)
{
        ei_point_t point = {center.x + rayon * cos(angle_depart * PI / 180), center.y - rayon * sin(angle_depart * PI / 180)};
        ei_linked_point_t *first_point = calloc(1, sizeof(ei_linked_point_t));
        float angle;
        first_point->point = point;
        ei_linked_point_t *previous = first_point;
        ei_linked_point_t *current = NULL;

        for (int i = 1; i < nb_points + 1; i++)
        {
                angle = (angle_depart + i * (angle_fin - angle_depart) / nb_points) * PI / 180;
                current = calloc(1, sizeof(ei_linked_point_t));
                point.x = (int)(center.x + rayon * cos(angle));
                point.y = (int)(center.y - rayon * sin(angle));
                current->point = point;
                previous->next = current;
                previous = current;
        }
        ei_extreme_linked_points_t *extreme_points = calloc(1, sizeof(ei_extreme_linked_points_t));
        extreme_points->head_point = first_point;
        extreme_points->tail_point = current;
        current->next = NULL;
        return extreme_points;
}

void ei_free_polygon(ei_linked_point_t **poly)
{
        if (poly != NULL)
        {
                ei_linked_point_t *del = *poly;
                while (*poly != NULL)
                {
                        *poly = (*poly)->next;
                        free(del);
                        del = *poly;
                }
        }
}
