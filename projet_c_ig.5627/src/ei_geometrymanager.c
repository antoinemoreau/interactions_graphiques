#include "ei_geometrymanager.h"

static ei_geometrymanager_t* geometrymanager_list = NULL;

void			ei_place			(ei_widget_t*		widget,
							 ei_anchor_t*		anchor,
							 int*			x,
							 int*			y,
							 int*			width,
							 int*			height,
							 float*			rel_x,
							 float*			rel_y,
							 float*			rel_width,
							 float*			rel_height) {

}

void ei_register_placer_manager(){

}

void ei_geometrymanager_unmap(){

}

ei_geometrymanager_t* ei_geometrymanager_from_name(ei_geometrymanager_name_t name){
	ei_geometrymanager_t* geometrymanager;
	for (ei_geometrymanager_t* current = geometrymanager_list; current != NULL; current = current->suivant){
		if (strcmp(current->name, geometrymanager->name)==0){
			return current;
		}
	}
	return NULL;
}

void ei_geometrymanager_register(ei_geometrymanager_t* geometrymanager){
	for (ei_geometrymanager_t* current = geometrymanager_list; current != NULL; current = current->next){
		if (strcmp(current->name, geometrymanager->name) == 0){
			return;
		}
	}
	geometrymanager->next = geometrymanager_list;
	geometrymanager_list = geometrymanager;
}
