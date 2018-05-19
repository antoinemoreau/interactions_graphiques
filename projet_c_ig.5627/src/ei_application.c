#include "ei_application.h"
#include "ei_widgetclass.h"
#include "ei_frame.h"
#include "ei_geometrymanager.h"
#include <stdio.h>

ei_widget_t *root;
ei_surface_t window;

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen) {
        hw_init();

        ei_frame_register_class();
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

        ei_widget_t* root = ei_app_root_widget ();
        ei_surface_t root_surface = ei_app_root_surface();

        hw_surface_lock(root_surface);
        root->wclass->drawfunc(root,root_surface,NULL,NULL);
        hw_surface_unlock(root_surface);
        hw_surface_update_rects(root_surface,NULL);
        getchar();
}

void ei_app_invalidate_rect(ei_rect_t* rect) {

}

void ei_app_quit_request() {

}

ei_widget_t* ei_app_root_widget() {
        return root;
}

ei_surface_t ei_app_root_surface() {
        return window;
}
