#include "ei_draw.h"
#include <stdio.h>

void ei_compute_color(ei_color_t initiale, ei_color_t* res, float variation){
        int rouge = (int) initiale.red*variation ;
        int bleu = (int) initiale.blue*variation;
        int vert = (int) initiale.green*variation;
        if (rouge < 255) {
                res->red = rouge;
        } else {
                res->red = 255;
        }
        if (bleu < 255) {
                res->blue = bleu;
        } else {
                res->blue = 255;
        }
        if (vert < 255) {
                res->green = vert;
        } else {
                res->green = 255;
        }
        res->alpha = initiale.alpha;
}
