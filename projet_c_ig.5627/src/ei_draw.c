#include "ei_draw.h"
#include "ei_draw_utils.h"
#include <stdio.h>
#include <stdlib.h>

void ei_draw_text(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t	color,
						 const ei_rect_t*	clipper){
	//la surface doit avoir été lock
	ei_rect_t rect_surface = hw_surface_get_rect(surface);
	ei_font_t font_text = font;
	if (font_text == NULL) {
		font_text = ei_default_font;
	}
	ei_surface_t surface_texte = hw_text_create_surface(text, font_text, color);
	//on lock la surface du text
	hw_surface_lock(surface_texte);
	//on recupere le rectangle de la surface de texte
	ei_rect_t rect_text = hw_surface_get_rect(surface_texte);
	rect_text.top_left = *where;
	//surface de destination
	ei_rect_t rect_dest = {*where,rect_text.size};
	if (clipper) {
		ei_intersection_rectangle(clipper,&rect_text,&rect_dest);
		rect_dest.top_left = *where;
		fprintf(stdout, "widht %d, height %d\n",rect_text.size.width, rect_text.size.height);
		fprintf(stdout, "%d, %d\n", rect_dest.size.width, rect_dest.size.height);
		//rect_text.size.width = rect_dest.size.width;
		//rect_text.size.height = rect_dest.size.height;
		fprintf(stdout, "widht %d, height %d\n",rect_text.size.width, rect_text.size.height);

	}
	//checking if the surface has an alpha parameter
	ei_bool_t alpha = hw_surface_has_alpha(surface_texte);
	//on sort en erreur si les dimmensions  ne correspondent pas
	if (ei_copy_surface(surface,&rect_dest,surface_texte,&rect_text,alpha)) {
		fprintf(stderr, "Different ROI size\n");
		exit(1);
	}
	hw_surface_unlock(surface_texte);
	hw_surface_free(surface_texte);
}
