#include "ei_draw.h"


void ei_fill	(ei_surface_t		surface,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper){
               hw_surface_lock(surface);
							 //definition de la couleur qui va être utilisé por remplir la zone
               unint32_t couleur = ei_map_rgba(surface,color);
							 //taille par defaut, celle de la surface tout entiere
               ei_size_t taille = hw_surface_get_size(surface);
							 //on recupere le pointeur vers le premier pixel
               uint8_t* premier_p = hw_surface_get_buffer(surface);
               if(clipper){
								 //definition de la longueur d'une ligne totale en mémoire
                 retour_ligne = taille.width*4;
								 //on met à jour la taille à celle de la zone du clipper
                 ei_size_t taille = clipper->size;
								 //on recupere la porsition du premier pixel de la zone
                 ei_point_t debut = clipper->top_left;
								 //on met à jouer le pointeur de depart
                 premier_p = premier_p + (debut.y*taille.height + debut.x)*4;
                 //pixels = taille.width * taille.height;
               }
               else
               {

                 //ei_point_t debut = {0,0};
                 //pixels = taille.width*taille.height;
                 retour_ligne =0;
               }
							 //double boucle pour remplir la zone voulue
               for (size_t y = 0; y < taille.height; y++) {
								 for (size_t x = 0; x < taille.width; x++) {
									 //on recupere l'adresse du pixel en position (x,y) dans la zone
									 uint8_t* pixel = premier_p + x*4;
									 //on modifie la couleur du pixel
									 *pixel = couleur;
								 }
								 //necessaire seulement si on est dans le clipper
                 premier_p +=  premier_p + retour_ligne;
               }
               hw_surface_unlock(surface);
							 hw_surface_update_rects(surface,clipper);

}
/*
void ei_draw_text(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t	color,
						 const ei_rect_t*	clipper){
hw_surface_lock(surface);
// uint8_t* first_p = hw_surface_get_buffer(surface); //on recupere l'adresse memoire du premier pixel de la surface bloqué
ei_surface_t surface_texte = hw_text_create_surface(text, font, color);
hw_surface_lock(surface_texte);

if (ei_copy_surface()) {
  fprintf(stderr, "Different ROI size\n");
  return 1;
} else {
}




hw_surface_unlock(surface_texte);
hw_surface_free(surface_texte);
hw_surface_unlock(surface);
//faire la mise à jour avec update rects
}
int ei_copy_surface(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha){


}
*/
