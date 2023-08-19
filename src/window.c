/*
 * window.c
 * Copyright (C) Nikolaos Kougioulis 2012-2013
 * 
 * Tympanus is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tympanus is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "variables.h"
#include "window.h"

/* Quit */
void tympanus_quit(GtkMenuItem *widget, gpointer data) {
	/* the pipeline must be stopped as well -
	 * wav files appeared to play even when the main_quit signal was called 
	 */
	 if (play) {
		 play = NULL;
		 gst_element_set_state(play, GST_STATE_NULL);
		 gtk_main_quit();
    }
	else {
		gtk_main_quit();
	}
	
}			

/* Creating the window */
 GtkWidget*
create_window (void) {
	accel_group = gtk_accel_group_new(); //accelerator group

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_app_paintable(window, TRUE);
	gtk_window_set_title(GTK_WINDOW(window), "Tympanus Media Player");
	gtk_window_set_default_size(GTK_WINDOW(window), 650, 450);
	gtk_window_set_icon_from_file(GTK_WINDOW(window), "src/images/icons/video32.png", NULL);

	
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

	
	/* signals & settings */
	g_object_set (gtk_settings_get_default (), "gtk-auto-mnemonics", FALSE, NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(tympanus_quit), NULL);
	
	return window;
}
