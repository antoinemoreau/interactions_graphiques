#include "ei_geometrymanager.h"
#include "ei_placer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static ei_geometrymanager_t* geometrymanager_list = NULL;

/*
On crée un geometrymanager de type placer en lui affectant
le nom et les fonctions associées à cette classe
*/
static ei_geometrymanager_t* create_placer_manager(){
	ei_geometrymanager_t* geometrymanager = calloc(1, sizeof(ei_geometrymanager_t));
	strncpy(geometrymanager->name, "placer", 20);
	geometrymanager->runfunc = (ei_geometrymanager_runfunc_t) &ei_placer_runfunc;
	geometrymanager->releasefunc = (ei_geometrymanager_releasefunc_t) &ei_placer_releasefunc;
	geometrymanager->next = NULL;
	return geometrymanager;
}


void ei_place (ei_widget_t* widget,
		 ei_anchor_t*		anchor,
		 int*			x,
		 int*			y,
		 int*			width,
		 int*			height,
		 float*			rel_x,
		 float*			rel_y,
		 float*			rel_width,
		 float*			rel_height) {

	 ei_placer_t* placer = NULL;
	 ei_geometrymanager_t* placer_manager = create_placer_manager();

	 //S'il n'y a pas de paramètre geom, on en crée et on remplie ensuite
	if (widget->geom_params == NULL){
		placer = calloc(1, sizeof(ei_placer_t));
		placer->manager = *placer_manager;
		widget->geom_params = placer;
	}
	widget->geom_params->manager = placer_manager;

	//On cast pour forcer les valeurs
	placer = (ei_placer_t*) (widget->geom_params);
	if (strcmp(widget->geom_params->manager->name, "placer") != 0 ){
		widget->geom_params->manager = placer_manager;
	}
	if (anchor != NULL){
		placer->anchor = *anchor;
	}
	else{
		placer->anchor = ei_anc_northwest;
	}
	if (x != NULL){
		placer->x = *x;
	}
	else{
		placer->x = 0;
	}
	if (y != NULL){
		placer->y = *y;
	}
	else{
		placer->y = 0;
	}
	if (width != NULL){
		placer->width = *width;
	}
	else{	//Si on a aucune taille (absolue ou relative) de fournie, on prend la taille minimale
		if (rel_width == NULL){
			placer->width = (widget->requested_size).width;
		}
		else{
			placer->width = 0;
		}
	}
	if (height != NULL){
		placer->height = *height;
	}
	else{	//Si on a aucune taille (absolue ou relative) de fournie, on prend la taille minimale
		if (rel_width == NULL){
			placer->height = (widget->requested_size).height;
		}
		else{
			placer->height = 0;
		}
	}
	if (rel_x != NULL){
		placer->rel_x = *rel_x;
	}
	else{
		placer->rel_x = 0.0;
	}
	if (rel_y != NULL){
		placer->rel_y = *rel_y;
	}
	else{
		placer->rel_y = 0.0;
	}
	if (rel_width != NULL){
		placer->rel_width = *rel_width;
	}
	else{
		placer->rel_width = 0.0;
	}
	if (rel_height != NULL){
		placer->rel_height = *rel_height;
	}
	else{
		placer->rel_height = 0.0;
	}

	ei_placer_runfunc(widget);
}



void ei_register_placer_manager(){
	if (geometrymanager_list != NULL){
		for (ei_geometrymanager_t* current = geometrymanager_list; current != NULL; current = current->next){
			if (strcmp(current->name, "placer") == 0){
				return;
			}
		}
	}
	ei_geometrymanager_register(create_placer_manager());
}

void ei_geometrymanager_unmap(ei_widget_t* widget){

}

ei_geometrymanager_t* ei_geometrymanager_from_name(ei_geometrymanager_name_t name){
	ei_geometrymanager_t* geometrymanager;
	for (ei_geometrymanager_t* current = geometrymanager_list; current != NULL; current = current->next){
		if (strcmp(current->name, name)==0){
			return current;
		}
	}
	return NULL;
}

void ei_geometrymanager_register(ei_geometrymanager_t* geometrymanager){
	geometrymanager->next = geometrymanager_list;
	geometrymanager_list = geometrymanager;
}
