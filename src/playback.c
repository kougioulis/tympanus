/*
 * playback.c
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

#include <gst/interfaces/xoverlay.h>
#include <gdk/gdkx.h> 
#include "variables.h"
#include "playback.h"

gdouble speed_value = 0.0;
gdouble normal_value = 1.0;
gdouble default_value = 1.0;

guint64 duration;
GstElement *x_overlay;
GstElement *pipeline;


/* method to display an alert message */
void alert(char *error_title, char *error_message) {
	  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            error_message);
  gtk_window_set_title(GTK_WINDOW(dialog), error_title);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}	
	
/* all functions related to the playback and pipeline */

/* print position method from gstreamer.freedesktop.org */
gboolean print_position(GstElement *play) {
  GstFormat fmt = GST_FORMAT_TIME;
  gint64 pos, len;

  if (gst_element_query_position(play, &fmt, &pos) && gst_element_query_duration(play, &fmt, &len)) {
    g_print("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r", GST_TIME_ARGS(pos), GST_TIME_ARGS(len));
    gtk_adjustment_set_value(GTK_ADJUSTMENT(progress), (pos*100)/len);

	//  char num = GST_TIME_FORMAT;
//gtk_label_set_text(toolbar_item.total_time_label, num);

  }

  return TRUE;
}
	
/* play */
void play_pipeline(gchar *addr) { 
	GstBus *bus;
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);

	play = gst_element_factory_make("playbin", "play");
	g_object_set(G_OBJECT(play), "uri", addr, NULL);

	bus = gst_pipeline_get_bus(GST_PIPELINE(play));
	gst_object_unref(bus);

  x_overlay = gst_element_factory_make("xvimagesink", "videosink");

  g_object_set(G_OBJECT(play), "video-sink", x_overlay, NULL);

  gst_x_overlay_set_window_handle(GST_X_OVERLAY(x_overlay), GDK_WINDOW_XID(video_area->window));
	
  g_timeout_add(200, (GSourceFunc) print_position, play);

  gtk_adjustment_set_value(GTK_ADJUSTMENT(progress), 0);

  gst_element_set_state(play, GST_STATE_PLAYING);

  /* when the video starts playing show the pause icon and set the pause tooltip */
  gtk_button_set_image(GTK_BUTTON(toolbar_item.play), gtk_image_new_from_stock(GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_SMALL_TOOLBAR));
  gtk_tooltips_set_tip(GTK_TOOLTIPS(tooltips.play), toolbar_item.play, "Pause the current item", NULL);
	
	/* the video is playing,so activate all buttons */
  	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.play), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.rewind), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.stop), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.forward), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.hscale), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.volume), TRUE);
    gtk_scale_button_set_value(GTK_SCALE_BUTTON(toolbar_item.volume), 0.75); //set volume to 75%
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.play), FALSE); //set "play" as disabled since the video automatically starts
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.pause), TRUE);

	/* menu items */
	gtk_widget_set_sensitive(menu_item.properties, TRUE);
	gtk_widget_set_sensitive(menu_item.zoom_in, TRUE);
	gtk_widget_set_sensitive(menu_item.zoom_normal, TRUE);
	gtk_widget_set_sensitive(menu_item.zoom_out, TRUE);
	gtk_widget_set_sensitive(menu_item.volume_up, TRUE);
	gtk_widget_set_sensitive(menu_item.volume_down, TRUE);
	gtk_widget_set_sensitive(menu_item.forward, TRUE);
	gtk_widget_set_sensitive(menu_item.rewind, TRUE);
	gtk_widget_set_sensitive(menu_item.speed_normal, TRUE);
	gtk_widget_set_sensitive(menu_item.slower, TRUE);
	gtk_widget_set_sensitive(menu_item.faster, TRUE);
	

	
	gtk_label_set_text(GTK_LABEL(status_label), "Playing...");
	
	g_main_loop_run(loop);

    gst_element_set_state(play, GST_STATE_NULL);
   gst_object_unref(GST_OBJECT(play));
}	

/* stop */
void stop(void) {
	if (play) {
			gst_element_set_state (play, GST_STATE_NULL);
			play = NULL;
			gtk_adjustment_set_value(GTK_ADJUSTMENT(progress), 0); /* set the value of the scale to zero */
		
    }
}	
/* reset */
void reset(void) { 
	    gst_element_seek (play, 1.0,
                      GST_FORMAT_TIME,
                      seek_flags,
                      GST_SEEK_TYPE_SET, 0,
                      GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}	

/* helpers for seek_cb */
void
backend_seek_absolute (guint64 value) {
	
    gst_element_seek (play, 1.0,
                      GST_FORMAT_TIME,
                      seek_flags,
                      GST_SEEK_TYPE_SET, value,
                      GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}	
guint64
backend_query_position (void) {
    GstFormat format = GST_FORMAT_TIME;
    guint64 cur;

    gst_element_query_position (play, &format, &cur);
    if (format != GST_FORMAT_TIME) {
        return GST_CLOCK_TIME_NONE;
	}

    return cur;
}	

guint64
backend_query_duration (void) {
    GstFormat format = GST_FORMAT_TIME;
    guint64 cur;

    gst_element_query_duration (play, &format, &cur);
    if (format != GST_FORMAT_TIME) {
		return GST_CLOCK_TIME_NONE;
}

    return cur;
}	
