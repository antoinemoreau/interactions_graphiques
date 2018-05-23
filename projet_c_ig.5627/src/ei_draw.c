#include "ei_draw.h"
#include "ei_draw_utils.h"
#include <stdio.h>
#include <stdlib.h>

uint32_t 		ei_map_rgba(ei_surface_t surface, const ei_color_t* color){
	int red_location;
	int green_location;
	int blue_location;
	int alpha_location;
	hw_surface_get_channel_indices(surface, &red_location, &green_location, &blue_location, &alpha_location);
	uint32_t rgba = 0;
	rgba += color->red << red_location*8;
	rgba += color->blue << blue_location*8;
	rgba += color->green << green_location*8;
	rgba += color->alpha << alpha_location*8;
	return rgba;
}


void 		ei_draw_text			(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t	color,
						 const ei_rect_t*	clipper){
	if(strcmp(text,"") == 0){
		return;
	}
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
	ei_rect_t clipper_new;
	if (clipper) {
		ei_intersection_rectangle(&rect_surface, clipper, &clipper_new);
		fprintf(stdout, "Rectangle frame : coordonnees (%d, %d) taille (%d, %d)\n", rect_surface.top_left.x, rect_surface.top_left.y,  rect_surface.size.width, rect_surface.size.height);
		fprintf(stdout, "Rectangle texte : coordonnees (%d, %d) taille (%d, %d)\n", clipper->top_left.x, clipper->top_left.y,  clipper->size.width, clipper->size.height);
		fprintf(stdout, "Coordonnees : (%d, %d) Taille : (%d, %d) \n ", clipper_new.top_left.x, clipper_new.top_left.y, clipper_new.size.width, clipper_new.size.height);
	}
	else{
		clipper_new = rect_surface;
	}
	ei_rect_t dest_maj;
	ei_intersection_rectangle(&clipper_new, &rect_dest, &dest_maj);

	rect_text.size = dest_maj.size;
	ei_copy_surface(surface, &dest_maj, surface_texte, &rect_text, alpha);
	hw_surface_unlock(surface_texte);
	hw_surface_free(surface_texte);
}
