#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"


/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x46, 0x82, 0xb4, 0xff};

	ei_widget_t*	frame;
	ei_widget_t*	frame2;
	ei_size_t	frame_size		= {300,200};
	ei_size_t	frame_size2		= {500,500};

	int		frame_x			= 150;
	int		frame_y			= 200;
	int		frame_x2		= 0;
	int		frame_y2		= 0;
	ei_color_t	frame_color		= {0x80, 0x80, 0x80, 0xff};
	ei_color_t	frame_color2		= {0xff, 0x00, 0x00, 0xff};
	ei_relief_t	frame_relief		= ei_relief_raised;
	int		frame_border_width	= 6;

	ei_widget_t* button2;
	ei_size_t	button_size2		= {400,400};
	int		button_x2		= 40;
	int		button_y2		= 40;
	ei_color_t	button_color2		= {0x00, 0x88, 0x00, 0xff};
	int		button_corner_radius2	= 6;
	ei_relief_t	button_relief2		= ei_relief_raised;
	int		button_border_width2	= 2;


	char* 		texte			= "Test long frame";
	ei_color_t      couleur_du_texte	= {0xff,0x14,0x93,0xff};
	ei_anchor_t 	ancre_text		= ei_anc_center;
	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	/* Create, configure and place the frame on screen. */
	frame = ei_widget_create("frame", ei_app_root_widget());
	ei_frame_configure(frame, &frame_size, &frame_color,
			    &frame_border_width, &frame_relief, &texte, NULL, &couleur_du_texte, &ancre_text,
			    NULL, NULL, NULL);
    	ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );


    	frame2 = ei_widget_create("frame", frame);
	ei_frame_configure(frame2, &frame_size2, &frame_color2,
			    &frame_border_width, &frame_relief, &texte, NULL, &couleur_du_texte, &ancre_text,
			    NULL, NULL, NULL);
	ei_place(frame2, NULL, &frame_x2, &frame_y2, NULL, NULL, NULL, NULL, NULL, NULL );


	button2 = ei_widget_create("button", frame2);
	ei_button_configure	(button2, &button_size2, &button_color2,
				 &button_border_width2, &button_corner_radius2, &button_relief2, NULL, NULL, NULL, NULL,
				 NULL, NULL, NULL, NULL, NULL);
	ei_place(button2, NULL, &button_x2, &button_y2, NULL, NULL, NULL, NULL, NULL, NULL );
	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
