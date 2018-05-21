#include "ei_draw.h"
#include "ei_draw_utils.h"
#include <stdio.h>
#include <stdlib.h>

void 		ei_draw_text			(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t	color,
						 const ei_rect_t*	clipper){
	ei_rect_t rect_surface = hw_surface_get_rect(surface);
	ei_font_t font_text = font;
	if (font_text == NULL) {
		font_text = ei_default_font;
	}
	ei_surface_t surface_texte = hw_text_create_surface(text, font_text, color);
	hw_surface_lock(surface_texte);//on lock la surface du text
	ei_rect_t rect_text = hw_surface_get_rect(surface_texte);//on recupere le rectangle de la surface de texte
	ei_bool_t alpha = hw_surface_has_alpha(surface_texte);
	ei_rect_t rect_dest = {*where,rect_text.size};
	if (clipper) {
		//ei_intersection_rectangle(&rect_surface, clipper,clipper);
	}
	ei_rect_t dest_maj;

	ei_intersection_rectangle(clipper,&rect_dest, &rect_dest);

	rect_text.size = rect_dest.size;
	ei_copy_surface(surface,&rect_dest,surface_texte,&rect_text,alpha);
	hw_surface_unlock(surface_texte);
	hw_surface_free(surface_texte);
}
