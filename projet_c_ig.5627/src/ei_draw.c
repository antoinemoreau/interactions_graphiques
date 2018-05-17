#include "ei_draw.h"

void ei_draw_text(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t	color,
						 const ei_rect_t*	clipper){
//la surface doit avoir été lock
uint8_t* first_p = hw_surface_get_buffer(surface); //on recupere l'adresse memoire du premier pixel de la surface lock
ei_surface_t surface_texte = hw_text_create_surface(text, font, color);
hw_surface_lock(surface_texte);
ei_bool_t alpha = hw_surface_has_alpha(surface);
if (ei_copy_surface()) {
	fprintf(stderr, "Different ROI size\n");
	return 1;
} else {
}




hw_surface_unlock(surface_texte);
hw_surface_free(surface_texte);
//unlock la surface
//faire la mise à jour avec update rects
}

int ei_copy_surface(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha){
if (dst_rect == NULL && src_rect ==NULL) {
	uint8_t* buffer_source = hw_surface_get_buffer(source);
	uint8_t* buffer_dest = hw_surface_get_buffer(destination);
	ei_size_t size_dest = hw_surface_get_size(destination);
	ei_size_t size_source = hw_surface_get_size(destination);
	if (size_dest.width !=size_source.width || size_source.height != size_dest.height){
		return 1;
	}else{
		for(int y = 0; y < taille.height; y++){
			for(int x = 0;x < taille.widht; x++){
				for(uint8_t i = 0; i<4,i++){
					buffer_dest += i;
					buffer_source += i;
					*buffer_dest = *buffer_source;
				}
			}
		}
		return 0;
	}
} else {
/* code */
}


}
