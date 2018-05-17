#include "ei_widgetclass.h"
#include "ei_frameclass.h"
#include <stdlib.h>
#include <string.h>


/*On crée un pointeur widgetclass_list qui nous permet de savoir quelle(s)
classe(s) de widget(s) existe(nt)
*/
static ei_widgetclass_t* widgetclass_list = NULL;

static ei_widgetclass_t* create_frame_widget(){
  ei_widgetclass_t *frame_widget = malloc(sizeof(ei_widgetclass_t));
  strncpy(frame_widget->name, "frameclass", 20);
  frame_widget->allocfunc = (ei_widgetclass_allocfunc_t) &ei_frameclass_allocfunc;
  frame_widget->releasefunc = (ei_widgetclass_releasefunc_t) &ei_frameclass_releasefunc;
  frame_widget->drawfunc = (ei_widgetclass_drawfunc_t) &ei_frameclass_drawfunc;
  frame_widget->setdefaultsfunc = (ei_widgetclass_setdefaultsfunc_t) &ei_frameclass_setdefaultsfunc;
  frame_widget->geomnotifyfunc = (ei_widgetclass_geomnotifyfunc_t) &ei_widgetclass_geomnotifyfunc;
  frame_widget->next = NULL;
  return frame_widget;
}


void			ei_frame_register_class 	() {
  if (widgetclass_list == NULL) {
    /*On crée le widget frame si la liste des classes de widgets est vide*/
    ei_widgetclass_register(create_frame_widget());
  }
  else{
    ei_widgetclass_t* current = widgetclass_list;
    while (current != NULL) {
      if ( strcmp(ei_widgetclass_stringname(current->name)), "frameclass" ) {
        return;
      }
      current = current->next;
    }
    /*On crée le widget frame si il n'est pas déjà dans la liste pour l'y ajouter*/
    ei_widgetclass_register(create_frame_widget());
  }
}

void			ei_button_register_class 	() {

}

void			ei_toplevel_register_class 	() {

}


void ei_widgetclass_register(ei_widgetclass_t* widgetclass){
  widgetclass->next = widgetclass_list;
  widgetclass_list = widgetclass;
}
