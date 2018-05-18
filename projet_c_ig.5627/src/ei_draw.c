#include "ei_draw.h"

void ei_draw_text(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t	color,
						 const ei_rect_t*	clipper){
//la surface doit avoir été lock
//on recupere l'adresse memoire du premier pixel de la surface lock
uint8_t* buffer_surface = hw_surface_get_buffer(surface);
//on recupere la surface du texte
if (font) {
	ei_surface_t surface_texte = hw_text_create_surface(text, font, color);
} else {
	ei_surface_t surface_texte = hw_text_create_surface(text, ei_default_font, color);
}
//on lock la surface du text
hw_surface_lock(surface_texte);
//on recupere les rectangles de la surface
ei_rect_t rect_text = hw_surface_get_rect(surface_texte);
//on recupere le buffer de la surface
uint8_t* buffer_text = hw_text_create_surface(text, font, color);
//checking if the surface has an alpha parameter
ei_bool_t alpha = hw_surface_has_alpha(surface);
//copie de la surface du text  l'endroit voulu
//on sort en erreur si les dimmensions  ne correspondent pas
if (ei_copy_surface(surface,clipper,surface_texte,rect_text,EI_FALSE)) {
	fprintf(stderr, "Different ROI size\n");
	exit(1);
}
hw_surface_unlock(surface_texte);
hw_surface_free(surface_texte);
//unlock la surface
//faire la mise à jour avec update rects
}
