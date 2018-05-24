#include "ei_application.h"
#include "ei_widgetclass.h"
#include "ei_frame.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"
#include "ei_event.h"
#include "ei_event_utils.h"
#include "ei_draw.h"
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
        ei_toplevel_register_class();
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
        ei_init_list_events ();
        struct ei_event_t* event = malloc(sizeof(struct ei_event_t));
        ei_linked_event_t* event_list = get_list_events();
        ei_widget_t* root = ei_app_root_widget ();
        ei_surface_t root_surface = ei_app_root_surface();
        ei_size_t pick_size;
        pick_size.width = root->screen_location.size.width;
        pick_size.height = root->screen_location.size.height;
        ei_surface_t pick_surface = hw_surface_create(root_surface, &pick_size, EI_FALSE);
        pick_size = hw_surface_get_size(pick_surface);
        ei_rect_t main_clipper;
        hw_surface_lock(root_surface);
        main_clipper = hw_surface_get_rect(root_surface);
        hw_surface_lock(pick_surface);
        ei_rect_t coucou = {main_clipper.top_left,{main_clipper.size.width -100,main_clipper.size.height -100 }};
        //on dessine tout les widgets en premier lieu
        draw_all_widgets(root, root_surface, pick_surface, &main_clipper, rect_list);
        // root->wclass->drawfunc(root, root_surface, pick_surface, root->content_rect);
        // root->children_head->wclass->drawfunc(root->children_head, root_surface, pick_surface, root->content_rect);
        // root->children_head->children_head->wclass->drawfunc(root->children_head->children_head, root_surface, pick_surface, root->children_head->content_rect);
        hw_surface_unlock(pick_surface);
        hw_surface_unlock(root_surface);
        hw_surface_update_rects(root_surface, NULL);

        //boucle des evenements

        while (!quit_app) {
                ei_widget_t* widget;

                hw_event_wait_next(event);

                switch(event->type) {
                        case ei_ev_app:
                                break;

                        case ei_ev_keydown:
                        case ei_ev_keyup:
                                widget = NULL;
                                handle_event(event_list, event, widget);
                                //redraw(root_surface, pick_surface, widget, rect_list);
                                break;

                        case ei_ev_mouse_buttondown:
                        case ei_ev_mouse_buttonup:
                        case ei_ev_mouse_move:
                                widget = mouse_capture(event, pick_surface, root);
                                handle_event(event_list, event, widget);
                                redraw(root_surface, pick_surface, widget, rect_list);
                                break;

                        case ei_ev_last:
                                break;

                        default:
                                break;
                }

                release_rect_list(&rect_list);
        }
        hw_surface_free(pick_surface);
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
