#include "ei_widgetclass.h"
#include "ei_frameclass.h"
#include <stdlib.h>
#include <string.h>


/*On crée un pointeur widgetclass_list qui nous permet de savoir quelle(s)
classe(s) de widget(s) existe(nt)
*/
static ei_widgetclass_t* widgetclass_list = NULL;

static ei_widgetclass_t* create_frame_widget(){
        ei_widgetclass_t *frame_widget = calloc(1, sizeof(ei_widgetclass_t));
        strncpy(frame_widget->name, "frame", 20);
        frame_widget->allocfunc = (ei_widgetclass_allocfunc_t) &ei_frame_allocfunc;
        frame_widget->releasefunc = (ei_widgetclass_releasefunc_t) &ei_frame_releasefunc;
        frame_widget->drawfunc = (ei_widgetclass_drawfunc_t) &ei_frame_drawfunc;
        frame_widget->setdefaultsfunc = (ei_widgetclass_setdefaultsfunc_t) &ei_frame_setdefaultsfunc;
        frame_widget->geomnotifyfunc = (ei_widgetclass_geomnotifyfunc_t) &ei_frame_geomnotifyfunc;
        frame_widget->next = NULL;
        return frame_widget;
}

static ei_widgetclass_t* create_button_widget(){
        ei_widgetclass_t *button_widget = calloc(1, sizeof(ei_widgetclass_t));
        strncpy(frame_widget->name, "button", 20);
        frame_widget->allocfunc = (ei_widgetclass_allocfunc_t) &ei_button_allocfunc;
        frame_widget->releasefunc = (ei_widgetclass_releasefunc_t) &ei_button_releasefunc;
        frame_widget->drawfunc = (ei_widgetclass_drawfunc_t) &ei_button_drawfunc;
        frame_widget->setdefaultsfunc = (ei_widgetclass_setdefaultsfunc_t) &ei_button_setdefaultsfunc;
        frame_widget->geomnotifyfunc = (ei_widgetclass_geomnotifyfunc_t) &ei_button_geomnotifyfunc;
        frame_widget->next = NULL;
        return button_widget;
}

ei_widgetclass_t* ei_widgetclass_from_name (ei_widgetclass_name_t name){
        for ( ei_widgetclass_t* current = widgetclass_list; current != NULL; current = current->next ) {
                if (strcmp(ei_widgetclass_stringname(current->name), ei_widgetclass_stringname(name)) == 0){
                        return current;
                }
        }
        return NULL;
}

void ei_frame_register_class () {
        if (widgetclass_list != NULL) {
                ei_widgetclass_t* current = widgetclass_list;
                while (current != NULL) {
                        if ( strcmp(ei_widgetclass_stringname(current->name), "frame") ) {
                                return;
                        }
                current = current->next;
                }
        }
        /*On crée le widget frame si il n'est pas déjà dans la liste*/
        ei_widgetclass_register(create_frame_widget());
}

void ei_button_register_class () {
        if (widgetclass_list != NULL) {
                ei_widgetclass_t* current = widgetclass_list;
                while (current != NULL) {
                        if ( strcmp(ei_widgetclass_stringname(current->name), "button") ) {
                                return;
                        }
                current = current->next;
                }
        }
        /*On crée le widget button si il n'est pas déjà dans la liste*/
        ei_widgetclass_register(create_button_widget());
}

void ei_toplevel_register_class () {

}


void ei_widgetclass_register (ei_widgetclass_t* widgetclass){
        widgetclass->next = widgetclass_list;
        widgetclass_list = widgetclass;
}
