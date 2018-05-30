#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_toplevel.h"
#include "ei_geometrymanager.h"


ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_callback_t button_callback = button_press;


ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
    printf("Click !\n");
    return EI_TRUE;
}

ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_sym == SDLK_ESCAPE) {
		ei_app_quit_request();
		return EI_TRUE;
	}

        return EI_FALSE;
}

int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size	= {1200, 1200};
        ei_color_t      root_bgcol      = {0x52, 0x7f, 0xb4, 0xff};

        ei_color_t	toplevel2_color	= {0x88, 0x88, 0x88, 0xff};
        int             toplevel2_border_width    = 4;
        char*           toplevel2_text    = "Premier fils";


        ei_size_t       window_size     = {400,400};
        char*           window_title    = "Deuxieem fils";
        ei_color_t      window_color    = {0x3d,0x4c,0x70, 0xff};
        int             window_border_width    = 2;
        ei_bool_t       closable        = EI_TRUE;
        ei_axis_set_t   window_resizable = ei_axis_both;
        ei_point_t	window_position	 = {10, 10};

        ei_point_t	window_position2 = {510, 510};


        ei_widget_t*    toplevel2;
        ei_widget_t*    toplevel;

	ei_app_create(&screen_size, EI_FALSE);
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);


        toplevel = ei_widget_create("toplevel", ei_app_root_widget());
        toplevel2 = ei_widget_create("toplevel", ei_app_root_widget());

        ei_toplevel_configure(toplevel, &window_size, &window_color, &window_border_width, &window_title, &closable, &window_resizable, NULL);
        ei_toplevel_configure(toplevel2, &window_size, &toplevel2_color, &toplevel2_border_width, &toplevel2_text, &closable, &window_resizable, NULL);


        ei_place(toplevel, NULL, &(window_position.x), &(window_position.y), NULL, NULL, NULL, NULL, NULL, NULL);
        ei_place(toplevel2, NULL, &(window_position2.x), &(window_position2.x), NULL, NULL, NULL, NULL, NULL, NULL);

	ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

        ei_app_run();

	ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);

	ei_app_free();

	return (EXIT_SUCCESS);
}
