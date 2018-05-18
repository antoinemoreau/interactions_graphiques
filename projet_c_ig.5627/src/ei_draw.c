#include "ei_draw.h"

void ei_draw_text(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t	color,
						 const ei_rect_t*	clipper){
//la surface doit avoir été lock
/*uint8_t* first_p = hw_surface_get_buffer(surface); //on recupere l'adresse memoire du premier pixel de la surface lock
ei_surface_t surface_texte = hw_text_create_surface(text, font, color);
hw_surface_lock(surface_texte);
ei_bool_t alpha = hw_surface_has_alpha(surface);
if (ei_copy_surface()) {
	fprintf(stderr, "Different ROI size\n");
	return 1;
} else {
}
*/



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
	//traitement des alphas
	if(alpha == 1){
		//TODO
	}else{
		//TODO
	}
        //parametre de la copie si les deux rectangles sont à NULL
	if (dst_rect == NULL && src_rect == NULL) {
		uint8_t* buffer_source = hw_surface_get_buffer(source);
		uint8_t* buffer_dest = hw_surface_get_buffer(destination);
		ei_size_t size_source = hw_surface_get_size(source);
		ei_size_t size_dest = hw_surface_get_size(destination);
		if (size_dest.width != size_source.width || size_dest.height != size_source.height) {
			return 1;
		} else {
				for(int y = 0; y < size_source.height; y++){
					for(int x = 0;x < size_source.widht; x++){
						for(uint8_t i = 0; i<4,i++){
							buffer_dest += i;
							source_pixel_f += i;
							*buffer_dest = *buffer_source;
						}
					}
				}
			return 0;//On renvoie que tout c'est bien passé durant la copie
		}
	//parametre de la copie si le rectangle source existe
	} else if (dst_rect == NULL && src_rect != NULL) {
		//pointeur des surfaces
		uint8_t* buffer_source = hw_surface_get_buffer(source);
		uint8_t* buffer_dest = hw_surface_get_buffer(destination);
		//size des surfaces
		ei_size_t size_source = hw_surface_get_size(source);
		ei_size_t size_dest = hw_surface_get_size(destination);
		//surface du rectangle de source
		ei_point_t first_point = src_rect->top_left;
		//mise a jour du pointeur du premier pixel de la source
		buffer_source = buffer_source + (first_point.y*size_source.height + first_point.x)*4;
		//necessaire pour revenir à la ligne
		int back_to_line = size_source.widht*4;
		//size du rectangle à parcourrir
		ei_size_t size_rect = src_rect->size;
		if (size_dest.width != size_rect.width || size_dest.height != size_rect.height) {
			return 1;
		} else {
				for(int y = 0; y < size_rect.height; y++){
					for(int x = 0;x < size_rect.widht; x++){
						for(uint8_t i = 0; i<4,i++){
							buffer_dest += i;
							source_pixel_f += i;
							*buffer_dest = *buffer_source;
						}
					}
					buffer_source += buffer_source + back_to_line;
				}
			return 0;//On renvoie que tout c'est bien passé durant la copie
		}
	//parametre de la copie si le rectangle de destination existe
	} else if (dst_rect != NULL && src_rect == NULL) {
		uint8_t* buffer_source = hw_surface_get_buffer(source);
		uint8_t* buffer_dest = hw_surface_get_buffer(destination);
		ei_size_t size_source = hw_surface_get_size(source);
		ei_size_t size_dest = hw_surface_get_size(destination);

		ei_point_t first_point = dst_rect->top_left;

		buffer_dest = buffer_dest + (first_point.y*size_dest.height + first_point.x)*4;

		int back_to_line = size_dest.widht*4;
		ei_size_t size_rect = dst_rect->size;
		if (size_rect.width != size_source.width || size_rect.height != size_source.height) {
			return 1;
		} else {
				for(int y = 0; y < size_rect.height; y++){
					for(int x = 0;x < size_rect.widht; x++){
						for(uint8_t i = 0; i<4,i++){
							buffer_dest += i;
							source_pixel_f += i;
							*buffer_dest = *buffer_source;
						}
					}
					buffer_dest += buffer_dest + back_to_line;
				}
			return 0;//On renvoie que tout c'est bien passé durant la copie
		}
	//parametre de la copie si les deux rectangles existent
	}else{
		uint8_t* buffer_source = hw_surface_get_buffer(source);
		uint8_t* buffer_dest = hw_surface_get_buffer(destination);
		ei_size_t size_source = hw_surface_get_size(source);
		ei_size_t size_dest = hw_surface_get_size(destination);

		ei_point_t first_point_dest = dst_rect->top_left;
		//surface du rectangle de source
		ei_point_t first_point_source = src_rect->top_left;
		//mise a jour du pointeur du premier pixel de la source
		buffer_source = buffer_source + (first_point_source.y*size_source.height + first_point_source.x)*4;
		buffer_dest = buffer_dest + (first_point_dest.y*size_dest.height + first_point_dest.x)*4;

		//necessaire pour revenir à la ligne
		int back_to_line_source = size_source.widht*4;
		//size du rectangle à parcourrir
		ei_size_t size_rect_source = src_rect->size;
		int back_to_line_dest = size_dest.widht*4;
		ei_size_t size_rect_dest = dst_rect->size;
		if (size_rect_dest.width != size_rect_source.width || size_rect_source.height != size_rect_dest.height) {
			return 1;
		} else {
				for(int y = 0; y < size_rect_source.height; y++){
					for(int x = 0;x < size_rect_source.widht; x++){
						for(uint8_t i = 0; i<4,i++){
							buffer_dest += i;
							source_pixel_f += i;
							*buffer_dest = *buffer_source;
						}
					}
					buffer_source += buffer_source + back_to_line_source;
					buffer_dest += buffer_dest + back_to_line_dest;
				}
			return 0;//On renvoie que tout c'est bien passé durant la copie
		}
	}
}
