/*
 * video_area.c
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

#include <gdk/gdkkeysyms.h>
#include "video_area.h"
#include "variables.h"
#include "playback.h" 

/*
 * video_area.c
 * Copyright (C) Nickolaos Modestos Kougioulis 
 * This file is part of Tympanus
 */

GtkWidget *video_area;
GtkWidget *window;
cairo_t *cr;

GtkWidget *right_play_pause;
GtkWidget *right_rewind;
GtkWidget *right_forward;
GtkWidget *right_fullscreen;

void show_menu_cb(GtkWidget *widget, gpointer data) {
	g_print("screen clicked");
}

/* rewind  */
void right_rewind_cb (GtkButton *rewind, gpointer data) {
			 
    guint64 to_seek;
	gdouble value = gtk_adjustment_get_value(progress);

    if (!DURATION_IS_VALID (duration))
				 duration = backend_query_duration ();

    if (!DURATION_IS_VALID (duration)) {
      alert("Error", "Duration is not valid");
	}

	/* Explanation of -duration / 6
	 * I substract the total duration divided by 6 so that i get 6 sections of the video 
	 *which will rewind the video back to the closest section */
	
	to_seek = (value / 100) * duration - (duration / 6);
	
	    gst_element_seek (play, 1.0,
                      GST_FORMAT_TIME,
                      seek_flags,
                      GST_SEEK_TYPE_SET, to_seek,
                      GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}

/* forward */
void right_forward_cb (GtkWidget *widget, gpointer data) {
			 
    guint64 to_seek;
	gdouble value = gtk_adjustment_get_value(progress);

    if (!DURATION_IS_VALID (duration))
				 duration = backend_query_duration ();

    if (!DURATION_IS_VALID (duration)) {
      alert("Error", "Duration is not valid");
	}

	/* Same implementation and method with rewind_cb but using + instead of - */
	
	to_seek = (value / 100) * duration + (duration / 6);
	
	    gst_element_seek (play, 1.0,
                      GST_FORMAT_TIME,
                      seek_flags,
                      GST_SEEK_TYPE_SET, to_seek,
                      GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}

void right_toggle_cb(GtkWidget *widget, gpointer data) {
	//using the classic true/false method to apply toggle effect to the button
	 static gboolean paused = FALSE; 
	
	if(paused) {
		gtk_button_set_image(widget, gtk_image_new_from_stock(GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_SMALL_TOOLBAR));
		gst_element_set_state(play, GST_STATE_PLAYING);
		gtk_tooltips_set_tip(GTK_TOOLTIPS(tooltips.play), GTK_WIDGET(toolbar_item.play), "Pause the current item", NULL); //set pause tooltip
		gtk_widget_set_sensitive(GTK_WIDGET(menu_item.play), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(menu_item.pause), TRUE);
		gtk_label_set_text(GTK_LABEL(status_label), "Playing");
		paused = FALSE;
	
	}
	
	else {
		gtk_button_set_image(widget, gtk_image_new_from_stock(GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_SMALL_TOOLBAR));
		gst_element_set_state(play, GST_STATE_PAUSED);
		gtk_tooltips_set_tip(GTK_TOOLTIPS(tooltips.play), GTK_WIDGET(toolbar_item.play), "Play the current item", NULL); //set play tooltip
		gtk_widget_set_sensitive(GTK_WIDGET(menu_item.play), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(menu_item.pause), FALSE);
		gtk_label_set_text(GTK_LABEL(status_label), "Paused");
		paused = TRUE;
	}
}
int video_area_toggle_cb(GtkWidget *video_area, GdkEventButton *event, gpointer data) { 
	//using the classic true/false method to apply toggle effect to the button
	 static gboolean paused = FALSE; 

	if(paused && play && (event->button == 1)) {
		gst_element_set_state(play, GST_STATE_PLAYING);
		gtk_tooltips_set_tip(GTK_TOOLTIPS(tooltips.play), GTK_WIDGET(toolbar_item.play), "Pause the current item", NULL); //set pause tooltip
		gtk_widget_set_sensitive(GTK_WIDGET(menu_item.play), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(menu_item.pause), TRUE);
		gtk_label_set_text(GTK_LABEL(status_label), "Playing");
		paused = FALSE;
		return TRUE;
		
	}

	
	else if(!(paused) && play && (event->button == 1)) {
		gst_element_set_state(play, GST_STATE_PAUSED);
		gtk_tooltips_set_tip(GTK_TOOLTIPS(tooltips.play), GTK_WIDGET(toolbar_item.play), "Play the current item", NULL); //set play tooltip
		gtk_widget_set_sensitive(GTK_WIDGET(menu_item.play), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(menu_item.pause), FALSE);
		gtk_label_set_text(GTK_LABEL(status_label), "Paused");
		paused = TRUE;
		return TRUE;
	}	
	/* right click */
	else if(event->button == 3) {
		GtkMenu *right_menu = gtk_menu_new();
		
		right_play_pause = gtk_image_menu_item_new_with_label("Play/Pause");
		gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(right_play_pause), gtk_image_new_from_stock(GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_MENU));
		gtk_widget_add_accelerator(right_play_pause, "activate", accel_group, GDK_KEY_space, 0, GTK_ACCEL_VISIBLE); 
		//gtk_widget_set_sensitive(GTK_WIDGET(right_play_pause), FALSE); //set disabled by default
		g_signal_connect(right_play_pause, "activate", G_CALLBACK(right_toggle_cb), NULL); 
		

		right_rewind = gtk_image_menu_item_new_with_label("Skip Backwards");
		gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(right_rewind), gtk_image_new_from_stock(GTK_STOCK_MEDIA_REWIND, GTK_ICON_SIZE_MENU));
		//gtk_widget_set_sensitive(GTK_WIDGET(right_rewind), FALSE); //set disabled by default
		g_signal_connect(right_rewind, "activate", G_CALLBACK(right_rewind_cb), NULL);
		
		right_forward = gtk_image_menu_item_new_with_label("Skip Forward");
		gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(right_forward), gtk_image_new_from_stock(GTK_STOCK_MEDIA_FORWARD, GTK_ICON_SIZE_MENU));
       // gtk_widget_set_sensitive(GTK_WIDGET(right_forward), FALSE); //set disabled by default
		g_signal_connect(right_forward, "activate", G_CALLBACK(right_forward_cb), NULL);

		GtkWidget *right_sep = gtk_separator_menu_item_new();

		/* append to menu */
		gtk_menu_shell_append(GTK_MENU_SHELL(right_menu), GTK_WIDGET(right_play_pause));
		gtk_menu_shell_append(GTK_MENU_SHELL(right_menu), GTK_WIDGET(right_rewind));
		gtk_menu_shell_append(GTK_MENU_SHELL(right_menu), GTK_WIDGET(right_forward));

		gtk_widget_show_all(GTK_WIDGET(right_menu));

		gtk_menu_popup(right_menu, NULL, NULL, NULL, NULL, 1, gtk_get_current_event_time());
	}
			
}

gboolean
video_area_expose(GtkWidget *video_area, GdkEvent *event, gpointer data) {
    //GdkPixbuf *pix;
    GError *err = NULL;
    /* Create pixbuf */
   // pix = gdk_pixbuf_new_from_file("/home/user/img.png", &err);
    if(err) {
        printf("Error : %s\n", err->message);
        g_error_free(err);
        return FALSE;
    }
	 int width, height;
    gtk_window_get_size(GTK_WINDOW(window), &width, &height);

    cr = gdk_cairo_create(video_area->window);

/* TODO: Capture screenshot */
//	if(save_pb == FALSE ) {
		// TODO: FIX alert("Error", "Could not capture screenshot");
	//}
  // gdk_cairo_set_source_pixbuf(cr, pix, 10, 10);
    cairo_paint(cr);
    cairo_fill (cr);
    cairo_destroy (cr);
    return FALSE;
}

/* Drawing Area */
/* creating the area where is video is going to be drawn */
GtkWidget*
create_video_area(void) {
	
	video_area = gtk_drawing_area_new();
	
	gtk_widget_add_events(video_area, GDK_BUTTON_PRESS_MASK); //adding the click event to the video area

	//set size request will not allow the video area to be hidden(by drag) competely
	//gtk_widget_set_size_request (video_area, 0x200, 0x100);
    gtk_widget_show_all(video_area);

	/* TODO: BUG: compiler error */
   // gtk_widget_realize(video_area); //create the GDK resources associated with the video_area so that we can use widget->window

	GdkColor color;
	gdk_color_parse ("black", &color);
	gtk_widget_modify_bg(video_area, GTK_STATE_NORMAL, &color);
	g_signal_connect(video_area, "expose-event", G_CALLBACK(video_area_expose), NULL);

	/* Clicking on video_area callback */
	g_signal_connect(video_area, "button-press-event", G_CALLBACK(video_area_toggle_cb), NULL);
	
	return video_area;
}
