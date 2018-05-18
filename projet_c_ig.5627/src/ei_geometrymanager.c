#include "ei_geometrymanager.h"
#include "ei_placer.h"
#include <stdlib.h>
#include <string.h>

static ei_geometrymanager_t* geometrymanager_list = NULL;


static ei_geometrymanager_t* create_placer_manager(){
	ei_geometrymanager_t* geometrymanager = calloc(sizeof(ei_geometrymanager_t), 1);
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

	 ei_placer_param_t* placer_param = NULL;
	 ei_geometrymanager_t* placer_manager = create_placer_manager();

	 //S'il n'y a pas de paramètre geom, on en crée et on remplie ensuite
	if (widget->geom_params == NULL){
		placer_param = calloc(sizeof(ei_placer_param_t), 1);
		widget->geom_params = placer_param;
	}

	if (widget->geom_params->manager == NULL){
		widget->geom_params->manager = placer_manager;
	}
	//On cast pour forcer les valeurs
	placer_param = (ei_placer_param_t*) (widget->geom_params);
	if (strcmp(widget->geom_params->manager->name, "placer") != 0 ){
		widget->geom_params->manager = placer_manager;
		ei_register_placer_manager();
	}
	if (anchor != NULL){
		placer_param->anchor = *anchor;
	}
	else{
		placer_param->anchor = ei_anc_northwest;
	}
	if (x != NULL){
		placer_param->x = *x;
	}
	else{
		placer_param->x = 0;
	}
	if (y != NULL){
		placer_param->y = *y;
	}
	else{
		placer_param->y = 0;
	}
	if (width != NULL){
		placer_param->width = *width;
	}
	else{	//Si on a aucune taille (absolue ou relative) de fournie, on prend la taille minimale
		if (rel_width == NULL){
			placer_param->width = (widget->requested_size).width;
		}
		else{
			placer_param->width = 0;
		}
	}
	if (height != NULL){
		placer_param->height = *height;
	}
	else{	//Si on a aucune taille (absolue ou relative) de fournie, on prend la taille minimale
		if (rel_width == NULL){
			placer_param->height = (widget->requested_size).height;
		}
		else{
			placer_param->height = 0;
		}
	}
	if (rel_x != NULL){
		placer_param->rel_x = *rel_x;
	}
	else{
		placer_param->rel_x = 0.0;
	}
	if (rel_y != NULL){
		placer_param->rel_y = *rel_y;
	}
	else{
		placer_param->rel_y = 0.0;
	}
	if (rel_width != NULL){
		placer_param->rel_width = *rel_width;
	}
	else{
		placer_param->rel_width = 0.0;
	}
	if (rel_height != NULL){
		placer_param->rel_height = *rel_height;
	}
	else{
		placer_param->rel_height = 0.0;
	}
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
