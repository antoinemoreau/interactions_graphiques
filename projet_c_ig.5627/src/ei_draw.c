#include "ei_draw.h"
#include <stdio.h>

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
		//on recupere la surface du texte
		font_text = ei_default_font;
	}
	ei_surface_t surface_texte = hw_text_create_surface(text, font_text, color);
	hw_surface_lock(surface_texte);
	//on lock la surface du text
	//on recupere le rectangle de la surface de texte
	ei_rect_t rect_text = hw_surface_get_rect(surface_texte);
	//surface de destination
	//checking if the surface has an alpha parameter
	if (clipper) {
		fprintf(stdout, "g pa fé enkore\n");
	}
	ei_bool_t alpha = hw_surface_has_alpha(surface_texte);
	//copie de la surface du text  l'endroit voulu
	//on sort en erreur si les dimmensions  ne correspondent pas
	ei_rect_t rect_dest = {*where,rect_text.size};
	if (ei_copy_surface(surface,&rect_dest,surface_texte,&rect_text,alpha)) {
		fprintf(stderr, "Different ROI size\n");
		exit(1);
	}
	ei_copy_surface(surface,&rect_dest,surface_texte,&rect_text,alpha);
	hw_surface_unlock(surface_texte);
	hw_surface_free(surface_texte);
}
