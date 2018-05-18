#include "ei_application.h"
#include "ei_widgetclass.h"
#include "ei_frame.h"
#include <stdio.h>

ei_widget_t *root;
ei_surface_t window;

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen) {
        hw_init();

        ei_frame_register_class();
        root = ei_widget_create("frame", NULL);

        root->requested_size = *main_window_size;

        ei_color_t color = {0xff, 0xff, 0xff, 0xff};
        ((ei_frame_t*)root)->color = &color;

        window = hw_create_window(main_window_size, fullscreen);
}

void ei_app_free() {

}

void ei_app_run() {
        //A.2 tentative de app run qui utilise les drawfunction des widgets
        /*ei_widget_t* current = root->children_head;
        ei_surface_t root_surface = ei_app_root_surface();
        while (current != NULL) {
                ei_widget_t* current_child = current->children_head;
                while (current_child) {
                        current_child->wclass->drawfunction();
                        current_child->next_sibling;
                }
                current = current->
        }*/
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
