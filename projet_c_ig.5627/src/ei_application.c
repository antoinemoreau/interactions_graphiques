#include "ei_application.h"
#include "ei_widgetclass.h"
#include "ei_frame.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"
#include "ei_event.h"
#include "ei_event_utils.h"
#include "ei_application_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ei_widget_t *root;
ei_surface_t window;
ei_bool_t quit_app = EI_FALSE;
ei_linked_rect_t* rect_list = NULL;

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen) {
        hw_init();

        ei_frame_register_class();
        ei_button_register_class();
        ei_register_placer_manager();

        root = ei_widget_create("frame", NULL);

        root->requested_size = *main_window_size;
        root->screen_location.size.width = main_window_size->width;
        root->screen_location.size.height = main_window_size->height;

        window = hw_create_window(main_window_size, fullscreen);
}

void ei_app_free() {

}

void ei_app_run() {
        //A.2 tentative de app run qui utilise les drawfunction des widgets
        struct ei_event_t* event = malloc(sizeof(struct ei_event_t));
        ei_linked_event_t* event_list = list_events();
        ei_widget_t* root = ei_app_root_widget ();
        ei_surface_t root_surface = ei_app_root_surface();
        ei_size_t pick_size;
        pick_size.width = root->requested_size.width;
        pick_size.height = root->requested_size.height;
        ei_surface_t pick_surface = hw_surface_create(root, &pick_size, EI_FALSE);
        ei_rect_t main_clipper;
        hw_surface_lock(root_surface);
        main_clipper = hw_surface_get_rect(root_surface);

        //on dessine tout les widgets en premier lieu
        draw_all_widgets(root, root_surface, pick_surface);

        hw_surface_unlock(root_surface);
        hw_surface_update_rects(root_surface, NULL);
        hw_surface_update_rects(pick_surface, NULL);

        //boucle des evenements
        while (!quit_app) {
                hw_event_wait_next(event);
                if (event->type != ei_ev_none){
                        //faut une fonction pour recup le widget
                        ei_widget_t* widget = NULL; //on met a nul pour pouvoir compiler
                        //remplacer null par la fonction qui recup le widget
                        char* widget_name = widget->wclass->name;
                        ei_bool_t no_callback = EI_FALSE;
                        ei_linked_event_t* current_event = event_list;
                        while(!no_callback && current_event){
                                if(current_event->tag){
                                        if (strcmp(current_event->tag,"all") == 0) {
                                                no_callback = (*(current_event->callback))(widget, event, current_event->user_param);
                                        } else if (strcmp(current_event->tag,widget_name) == 0) {
                                                no_callback = (*(current_event->callback))(widget, event, current_event->user_param);
                                        }
                                }else if (current_event->widget == widget){
                                        no_callback = (*(current_event->callback))(widget, event, current_event->user_param);
                                }
                                current_event = current_event->next;
                        }
                        widget->wclass->drawfunc(widget, root_surface, pick_surface, widget->content_rect);
                        rect_list->rect = *(widget->content_rect);
                        rect_list->next = NULL;
                        ei_widget_t* current_child = widget->children_head;
                        while (current_child) {
                                current_child->wclass->drawfunc(current_child, root_surface, pick_surface, current_child->content_rect);
                                ei_linked_rect_t* rect_child;
                                rect_child->rect = *(current_child->content_rect);
                                rect_child->next = NULL;
                                rect_list->next = rect_child;
                                current_child = current_child->next_sibling;
                        }
                }
                hw_surface_update_rects(root_surface,rect_list);
                hw_surface_update_rects(pick_surface,rect_list);
                //faut vider la liste des rectangles
                rect_list =  NULL;
        }
        free(event);
}

void ei_app_invalidate_rect(ei_rect_t* rect) {

}

void ei_app_quit_request() {
        quit_app = EI_TRUE;
}

ei_widget_t* ei_app_root_widget() {
        return root;
}

ei_surface_t ei_app_root_surface() {
        return window;
}
