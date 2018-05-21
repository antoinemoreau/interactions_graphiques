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
	ei_rect_t clipper_new;
	if (clipper) {
<<<<<<< HEAD
		//ei_intersection_rectangle(&rect_surface, clipper,clipper);
	}
	ei_rect_t dest_maj;

	ei_intersection_rectangle(clipper,&rect_dest, &rect_dest);

	rect_text.size = rect_dest.size;
	ei_copy_surface(surface,&rect_dest,surface_texte,&rect_text,alpha);
=======
		fprintf(stdout, "x: %d, y:%d, widht: %d, height: %d\n", rect_surface.top_left.x,rect_surface.top_left.y,rect_surface.size.width, rect_surface.size.height );
		fprintf(stdout, "x: %d, y:%d, widht: %d, height: %d\n", clipper->top_left.x,clipper->top_left.y,clipper->size.width, clipper->size.height );
		ei_intersection_rectangle(&rect_surface, clipper, &clipper_new);
	}else{
		clipper_new = rect_surface;
	}
	ei_rect_t dest_maj;
	fprintf(stdout, "x: %d, y:%d, widht: %d, height: %d\n", clipper_new.top_left.x,clipper_new.top_left.y,clipper_new.size.width, clipper_new.size.height );
	fprintf(stdout, "x: %d, y:%d, widht: %d, height: %d\n", rect_dest.top_left.x,rect_dest.top_left.y,rect_dest.size.width, rect_dest.size.height );
	ei_intersection_rectangle(&clipper_new, &rect_dest, &dest_maj);
	fprintf(stdout, "x: %d, y:%d, widht: %d, height: %d\n", dest_maj.top_left.x,dest_maj.top_left.y,dest_maj.size.width, dest_maj.size.height );


	rect_text.size = dest_maj.size;
	ei_copy_surface(surface, &dest_maj, surface_texte, &rect_text, alpha);
>>>>>>> 9d544a77a9b7c2e2096ca08bb63f0ee1be96812b
	hw_surface_unlock(surface_texte);
	hw_surface_free(surface_texte);
}
