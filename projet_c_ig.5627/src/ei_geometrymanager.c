#include "ei_geometrymanager.h"
#include "ei_placer.h"
#include <stdlib.h>
#include <string.h>

static ei_geometrymanager_t* geometrymanager_list = NULL;

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

}

static ei_geometrymanager_t* create_placer_manager(){
	ei_geometrymanager_t* geometrymanager = malloc(sizeof(ei_geometrymanager_t));
	strncpy(geometrymanager->name, "placer", 20);
	geometrymanager->runfunc = (ei_geometrymanager_runfunc_t) &ei_placer_runfunc;
	geometrymanager->releasefunc = (ei_geometrymanager_releasefunc_t) &ei_placer_releasefunc;
	geometrymanager->next = NULL;
	return geometrymanager;
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
