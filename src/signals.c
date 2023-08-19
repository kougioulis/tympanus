#include "signals.h"
#include "variables.h"

/*
 * signals.c
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

GstElement *src, *sink;


/* play/pause pipeline - FOR PLAY/PAUSE MENU ITEMS */
void menu_pause_toggle_cb(GtkMenuItem *widget, gpointer data) {
	//using the classic true/false method to apply toggle effect to the button
	 static gboolean paused = FALSE; 
	
	if(paused) {
		gst_element_set_state(play, GST_STATE_PLAYING);
		gtk_widget_set_sensitive(menu_item.play, FALSE);
		gtk_widget_set_sensitive(menu_item.pause, TRUE);
		/* change the toolbar's icon as well */
		gtk_button_set_image(GTK_BUTTON(toolbar_item.play), gtk_image_new_from_stock(GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_SMALL_TOOLBAR));

		paused = FALSE;
	}
	
	else {
		gst_element_set_state(play, GST_STATE_PAUSED);
		gtk_widget_set_sensitive(menu_item.play, TRUE);
		gtk_widget_set_sensitive(menu_item.pause, FALSE);
		/* same thing */
		gtk_button_set_image(GTK_BUTTON(toolbar_item.play), gtk_image_new_from_stock(GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_SMALL_TOOLBAR));
		paused = TRUE;
	}
}
