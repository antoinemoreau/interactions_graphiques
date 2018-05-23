#include "ei_application.h"
#include "ei_widgetclass.h"
#include "ei_frame.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"
#include "ei_event.h"
#include "ei_event_utils.h"
#include "ei_draw.h"
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
        printf("%d\n",root->screen_location.size.width);
        ei_surface_t pick_surface = hw_surface_create(root_surface, &pick_size, EI_FALSE);
        pick_size = hw_surface_get_size(pick_surface);
        ei_rect_t main_clipper;
        hw_surface_lock(root_surface);
        main_clipper = hw_surface_get_rect(root_surface);
        hw_surface_lock(pick_surface);
        //on dessine tout les widgets en premier lieu
        draw_all_widgets(root, root_surface, pick_surface, root->content_rect, rect_list);

        hw_surface_unlock(pick_surface);
        hw_surface_unlock(root_surface);
        hw_surface_update_rects(root_surface, rect_list);
        //hw_surface_update_rects(pick_surface, NULL);
        //boucle des evenements
        /*
        while (!quit_app) {
                hw_event_wait_next(event);
                if (event->type != ei_ev_none){
                        ei_point_t where_mouse = event->param.mouse.where;
                        ei_rect_t rect_pick = hw_surface_get_rect(pick_surface);
                        hw_surface_lock(pick_surface);
                        uint8_t* buffer_picking = hw_surface_get_buffer(pick_surface);
                        hw_surface_unlock(pick_surface);
                        buffer_picking += (where_mouse.x + where_mouse.y*rect_pick.size.width)*4;
                        ei_color_t mouse_color = {*(buffer_picking+2),*(buffer_picking+1),*(buffer_picking),*(buffer_picking+3)};
                        uint32_t mouse_id = ei_map_rgba(pick_surface, &mouse_color);
                        ei_widget_t* widget = ei_pick_widget(mouse_id, root);
                        char* widget_name = widget->wclass->name;
                        ei_bool_t no_callback = EI_FALSE;
                        ei_linked_event_t* current_event = event_list;
                        while(!no_callback && current_event){
                                if(current_event->tag){
                                        if (strcmp(current_event->tag,"all") == 0) {
                                                no_callback = (*(current_event->callback))(widget, event, current_event->user_param);
                                        } else if (strcmp(current_event->tag, widget_name) == 0) {
                                                no_callback = (*(current_event->callback))(widget, event, current_event->user_param);
                                        }
                                }else if (current_event->widget == widget){
                                        no_callback = (*(current_event->callback))(widget, event, current_event->user_param);
                                }
                                current_event = current_event->next;
                        }
                        hw_surface_lock(root_surface);
                        hw_surface_lock(pick_surface);
                        draw_all_widgets(widget, root_surface, pick_surface, widget->content_rect, rect_list);
                        hw_surface_unlock(pick_surface);
                        hw_surface_unlock(root_surface);
                        hw_surface_update_rects(root_surface,rect_list);
                        //hw_surface_update_rects(pick_surface,rect_list);
                }
                //faut vider la liste des rectangles
        }
        hw_surface_free(pick_surface);
        free(event);
        */
        getchar();
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
