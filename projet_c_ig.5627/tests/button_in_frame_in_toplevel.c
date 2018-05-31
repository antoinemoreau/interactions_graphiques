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

        ei_size_t       window_size     = {800,800};
        char*           window_title    = "Hello World !";
        ei_color_t      window_color    = {0x3d,0x4c,0x70, 0xff};
        int             window_border_width    = 4;
        ei_bool_t       closable        = EI_TRUE;
        ei_axis_set_t   window_resizable = ei_axis_both;
        ei_point_t	window_position	 = {30, 10};

        ei_widget_t*	frame;
	ei_size_t	frame_size		= {600,400};
	int		frame_x			= 150;
	int		frame_y			= 200;
	ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
	ei_relief_t	frame_relief		= ei_relief_raised;
	int		frame_border_width	= 6;

        ei_widget_t*	button;
	ei_size_t	button_size		= {400,40};
	int		button_x		= 150;
	int		button_y		= 200;
	ei_color_t	button_color		= {0x00, 0x88, 0x88, 0xff};
	char*		button_title		= "Mon premier Bouton !";
	ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
	int		button_corner_radius	= 20;
	ei_relief_t	button_relief		= ei_relief_raised;
	int		button_border_width	= 2;
	ei_callback_t	button_callback 	= button_press;


        ei_widget_t*    toplevel;

	ei_app_create(&screen_size, EI_FALSE);
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        toplevel = ei_widget_create("toplevel", ei_app_root_widget());
        ei_toplevel_configure(toplevel, &window_size, &window_color, &window_border_width, &window_title, &closable, &window_resizable, NULL);
        ei_place(toplevel, NULL, &(window_position.x), &(window_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

        frame = ei_widget_create("frame", toplevel);
	ei_frame_configure(frame, &frame_size, &frame_color,
			    &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
			    NULL, NULL, NULL);
	ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );

        button = ei_widget_create("button", frame);
	ei_button_configure	(button, &button_size, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, &button_title, NULL, &button_text_color, NULL,
				 NULL, NULL, NULL, &button_callback, NULL);
	ei_place(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

	ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

        ei_app_run();

	ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);

	ei_app_free();

	return (EXIT_SUCCESS);
}
