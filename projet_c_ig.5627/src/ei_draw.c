#include "ei_draw.h"

void ei_draw_text(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t	color,
						 const ei_rect_t*	clipper){
//la surface doit avoir été lock
ei_rect_t rect_surface = hw_surface_get_rect(surface);
if (font) {
	//on recupere la surface du texte
	ei_surface_t surface_texte = hw_text_create_surface(text, font, color);
} else {
	ei_surface_t surface_texte = hw_text_create_surface(text, ei_default_font, color);
}
//on lock la surface du text
hw_surface_lock(surface_texte);
//on recupere le rectangle de la surface de texte
ei_rect_t rect_text = hw_surface_get_rect(surface_texte);
//surface de destination
//checking if the surface has an alpha parameter
if (clipper) {
	/* code */
}
ei_bool_t alpha = hw_surface_has_alpha(surface_texte);
//copie de la surface du text  l'endroit voulu
//on sort en erreur si les dimmensions  ne correspondent pas
if (ei_copy_surface(surface,clipper,surface_texte,rect_text,alpha)) {
	fprintf(stderr, "Different ROI size\n");
	exit(1);
}
ei_rect_t rect_dest = {*where,rect_text.size};
ei_copy_surface();
hw_surface_unlock(surface_texte);
hw_surface_free(surface_texte);
}
