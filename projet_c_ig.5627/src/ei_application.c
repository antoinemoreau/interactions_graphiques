#include "ei_application.h"
#include "ei_application_utils.h"
#include "ei_widgetclass.h"
#include "ei_frame.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"
#include "ei_event.h"
#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_draw_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ei_widget_t *root;
ei_surface_t root_surface;
ei_bool_t quit_app = EI_FALSE;

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen) {
        hw_init();

        ei_frame_register_class();
        ei_button_register_class();
        ei_toplevel_register_class();
        ei_register_placer_manager();


        root_surface = hw_create_window(main_window_size, fullscreen);

        ei_size_t pick_size;
        pick_size.width = main_window_size->width;
        pick_size.height = main_window_size->height;
        pick_surface = hw_surface_create(root_surface, &pick_size, EI_FALSE);

        root = ei_widget_create("frame", NULL);
        root->requested_size = *main_window_size;
        root->screen_location.size.width = main_window_size->width;
        root->screen_location.size.height = main_window_size->height;
}

void ei_app_free() {
        // Destruction de l'arborescence de widgets
        ei_widget_destroy(root);

        // Destruction de la liste d'événements
        free_list_events();

        // Libération des surfaces
        hw_surface_free(root_surface);
        hw_surface_free(pick_surface);
}

void ei_app_run() {
        ei_init_list_events ();
        struct ei_event_t* event = malloc(sizeof(struct ei_event_t));
        ei_linked_event_t* event_list = get_list_events();
        ei_rect_t main_clipper;
        hw_surface_lock(root_surface);
        main_clipper = hw_surface_get_rect(root_surface);
        hw_surface_lock(pick_surface);
        //on dessine tout les widgets en premier lieu
        draw_all_widgets(root, root_surface, pick_surface, &main_clipper);
        hw_surface_unlock(pick_surface);
        hw_surface_unlock(root_surface);
        hw_surface_update_rects(root_surface, NULL);

        //boucle des evenements

        while (!quit_app) {
                ei_widget_t* widget;
                ei_widget_t* parent;
                hw_event_wait_next(event);

                ei_point_t mouse_where = event->param.mouse.where;
                switch(event->type) {
                        case ei_ev_app:
                                break;

                        case ei_ev_keydown:
                        case ei_ev_keyup:
                                widget = NULL;
                                handle_event(event_list, event, widget);
                                drawing = EI_FALSE;
                                break;

                        case ei_ev_mouse_buttondown:
                        case ei_ev_mouse_buttonup:
                        case ei_ev_mouse_move:
                                widget = ei_widget_pick(&mouse_where);
                                parent = widget->parent;

                                handle_event(event_list, event, widget);
                                break;

                        case ei_ev_last:
                                break;

                        default:
                                break;
                }
                ei_linked_rect_t** rect_list = get_rect_list();
                if(drawing){
                        ei_rect_t new_rect;
                        ei_rect_t* clipper = &(root->screen_location);

                        if (destroy) {
                                widget = parent;
                                destroy = EI_FALSE;
                        }

                        if (parent)
                                clipper = &(parent->screen_location);

                        ei_intersection_rectangle(clipper, &(widget->screen_location), &new_rect);
                        rect_list_add(rect_list, new_rect);
                        //printf("coucou le redraw\n");
                        redraw(root_surface, pick_surface, widget, *rect_list);
                        drawing = EI_FALSE;
                }
                release_rect_list(rect_list);
        }
        //hw_surface_free(pick_surface);
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
        return root_surface;
}
