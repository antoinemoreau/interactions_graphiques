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
        //copie si les deux rectangles sont à NULL
	if (dst_rect == NULL && src_rect == NULL) {
		uint8_t* buffer_source = hw_surface_get_buffer(source);
		uint8_t* buffer_dest = hw_surface_get_buffer(destination);
		ei_size_t size_dest = hw_surface_get_size(destination);
		ei_size_t size_source = hw_surface_get_size(destination);
		//on verifie que les dimensions correspondent
		if (size_dest.width !=size_source.width || size_source.height != size_dest.height){
			return 1; //on renvoie le code d'erreur qui sera gérer
		}else{
			//on regarde la valeur du parametre alpha
			if (alpha == 1) {
				for(int y = 0; y < size_source.height; y++){
					for(int x = 0;x < size_source.widht; x++){
						for(uint8_t i = 0; i<4,i++){
							buffer_dest += i;
							buffer_source += i;
							*buffer_dest = *buffer_source;
						}
					}
				}
			} else {
				//cas si le parametre alpha est à True
				for(int y = 0; y < size_source.height; y++){
					for(int x = 0;x < size_source.widht; x++){
						for(uint8_t i = 0; i<4,i++){
							buffer_dest += i;
							buffer_source += i;
							*buffer_dest = *buffer_source;
						}
					}
				}
			}
			return 0;//n renvoie que tout c'est bien passé durant la copie

		}
	} else if (dst_rect == NULL && src_rect != NULL) {
		ei_size_t size_rect_source = src_rect->size;
		if (size_dest.width != size_rect_source.width || size_dest.height != size_rect_source.height) {
			return 1;
		} else {
			ei_point_t first_point = src_rect->top_left;
			uint8_t* source_pixel_f = buffer_source + (first_point.y*size_source.height + first_point.x)*4;
			if (alpha == 1) {
				for(int y = 0; y < size_rect_source.height; y++){
					for(int x = 0;x < size_rect_source.widht; x++){
						for(uint8_t i = 0; i<4,i++){
							buffer_dest += i;
							source_pixel_f += i;
							*buffer_dest = *source_pixel_f;
						}
					}
					source_pixel_f += source_pixel_f + size_source.widht*4
				}
			} else {
				//cas si le parametre alpha est à True
				for(int y = 0; y < .height; y++){
					for(int x = 0;x < .widht; x++){
						for(uint8_t i = 0; i<4,i++){
							buffer_dest += i;
							buffer_source += i;
							*buffer_dest = *buffer_source;
						}
					}
				}
			}
			return 0;//On renvoie que tout c'est bien passé durant la copie
		}
	}else if(dst_rect != NULL && src_rect == NULL){
		ei_size_t size_rect_dst = dst_rect->size;
		if (size_rect_dst.widht != src_rect.widht || size_rect_dst.height != src_rect.height){
			return 1;
		}else {

			return 0;
		}

	}


}
