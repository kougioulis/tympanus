/*
 * menubar.c
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
#include "menubar.h"
#include "signals.h"
#include "playback.h"
#include "about_dialog.h"
#include "variables.h"

double speed_value;
double normal_value;
double default_value;

GstElement *play;
GtkWidget *menubar;

GstSeekFlags seek_flags = GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT;

GdkPixbuf *get_screenshot() {
    GdkPixbuf *screenshot;
    GdkWindow *root_window;
    gint x_orig, y_orig;
    gint width, height;
    root_window = gdk_get_default_root_window ();
    gdk_drawable_get_size (root_window, &width, &height);      
    gdk_window_get_origin (root_window, &x_orig, &y_orig);

    screenshot = gdk_pixbuf_get_from_drawable (NULL, root_window, NULL,
                                           x_orig, y_orig, 0, 0, width, height);
    return screenshot;
}

gboolean export_pixmap_to_jpeg(GdkPixbuf *pixbuf, gchar *filename) {
	
//  GdkPixbuf *pixbuf;
  GdkPixmap *pixmap;
  GtkWidget *canvas;
	
  //canvas= lookup_widget (GTK_WIDGET(video_area), "canvas");
  //pixmap = g_object_get_data(G_OBJECT(video_area),"pixmap");

  gdk_pixbuf_save(pixbuf,filename,"jpeg",NULL,"quality","100",NULL);
	
  g_object_unref(pixbuf);
  return TRUE;
}



/* take screenshot */
void screenshot_cb(GtkWidget *widget, gpointer data) {

   static GtkWidget *dialog = NULL;
   GdkPixbuf *pixbuf = get_screenshot();

   if( ! dialog ) {
	   
      dialog = gtk_file_chooser_dialog_new( "Save Screenshot...",
                                   GTK_WINDOW(window),
                                   GTK_FILE_CHOOSER_ACTION_SAVE,
                                   GTK_STOCK_CANCEL, 0,
                                   GTK_STOCK_SAVE, 1,
                                   NULL );
   }

   if( 1 == gtk_dialog_run(GTK_DIALOG(dialog))) {
      /* Save file */
      gchar *filename;

      filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER( dialog ) );
	   export_pixmap_to_jpeg(pixbuf, filename);
      //cairo_surface_write_to_png(cr, filename );
      g_free( filename );
   }

   gtk_widget_hide( dialog );
}

/* normal speed signal */
void pipeline_normal_speed_cb(GtkWidget *widget, gpointer data) { 
	speed_value = 0.0;
	    gst_element_seek (play, default_value,
                      GST_FORMAT_TIME,
                      seek_flags,
                      GST_SEEK_TYPE_SET, backend_query_position(),
                      GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
	
	normal_value = default_value + speed_value;

}	
/* fast speed signal */
void pipeline_faster_speed_cb(GtkWidget *widget, gpointer data) { 
	speed_value = 0.3;
	
	    gst_element_seek (play, normal_value + speed_value,
                      GST_FORMAT_TIME,
                      seek_flags,
                      GST_SEEK_TYPE_SET, backend_query_position(),
                      GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
	
	normal_value = default_value + speed_value;
	
}	
/* slow speed signal */
void pipeline_slower_speed_cb(GtkWidget *widget, gpointer data) { 
	speed_value = 0.3;
	    gst_element_seek (play, normal_value - speed_value,
                      GST_FORMAT_TIME,
                      seek_flags,
                      GST_SEEK_TYPE_SET, backend_query_position(),
                      GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
	
	normal_value = default_value - speed_value;
}
	
/* Quit */
void menubar_tympanus_quit(GtkMenuItem *widget, gpointer data) {
	/* the pipeline must be stopped as well -
	 * wav files appeared to play even when the main_quit signal was called 
	 */
	 if (play) {
		 play = NULL;
		 	gtk_main_quit();
    }
	else {
		gtk_main_quit();
	}
	
}	

/* view -> menubar */
void show_menubar_cb(GtkMenuItem *widget, gpointer data) { 
	if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) { 
		gtk_widget_show(menubar);
	}
	else {
		gtk_widget_hide(menubar);
	}
}	
/* view -> controls */
void show_toolbar_cb(GtkMenuItem *widget, gpointer data) { 
	if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		gtk_widget_show(toolbar);
	}
	else {
		gtk_widget_hide(toolbar);
	}
}	
void show_statusbar_cb(GtkMenuItem *widget, gpointer data) { 
	if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		gtk_widget_show(statusbar);
		gtk_widget_show(separator_hbox); //also show the separator
	}
	else {
		gtk_widget_hide(statusbar);
		gtk_widget_hide(separator_hbox); //also hide the separator
	}
}
/* show/hide video area */
void hide_video_area_cb(GtkMenuItem *widget, gpointer data) { 
	if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		gtk_widget_show(video_area);
	}
	else {
		gtk_widget_hide(video_area);
	}
}	
/* toggle fullscreen interface */
void fullscreen_interface_cb(GtkMenuItem *widget, gpointer data) {
	static gboolean fullscreen = FALSE;

	/* this callback only fullscreens/unfulscreens the window; the bars are still visible */
	if(fullscreen) {
		gtk_window_unfullscreen(GTK_WINDOW(window));
		
        fullscreen = FALSE;
    }
    else {
        gtk_window_fullscreen(GTK_WINDOW(window));
		
        fullscreen = TRUE;
    }
}	
/* toggle fullscreen FOR MENU ITEM*/
void menu_fullscreen_interface_cb(GtkMenuItem *widget, gpointer data) {
	
	if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {

		gtk_window_fullscreen(GTK_WINDOW(window));
    }
    else {
        gtk_window_unfullscreen(GTK_WINDOW(window));
    }
}	
/* rewind  */
void
menubar_rewind_cb (GtkButton *widget, gpointer data) {
			 
    guint64 to_seek;
	gdouble value = gtk_adjustment_get_value(progress);

    if (!DURATION_IS_VALID (duration))
				 duration = backend_query_duration ();

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
void
menubar_forward_cb (GtkMenuItem *widget, gpointer data) {
			 
    guint64 to_seek;
	gdouble value = gtk_adjustment_get_value(progress);

    if (!DURATION_IS_VALID (duration)) 
				 duration = backend_query_duration ();


	/* Same implementation and method with rewind_cb but using + instead of - */
	
	to_seek = (value / 100) * duration + (duration / 6);
	
	    gst_element_seek (play, 1.0,
                      GST_FORMAT_TIME,
                      seek_flags,
                      GST_SEEK_TYPE_SET, to_seek,
                      GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}
	
/* volume up */
void volume_up_cb(GtkMenuItem *widget, gpointer data) {
gdouble volume = gtk_scale_button_get_value(GTK_SCALE_BUTTON(toolbar_item.volume));

gdouble new_volume = (volume + 0.08);

/* values under 0.0 and over 1.0 are not allowed */
if((new_volume > 0.0) && (new_volume < 1.0)) { 
g_object_set (play, "volume", new_volume , NULL); 
gtk_scale_button_set_value(GTK_SCALE_BUTTON(toolbar_item.volume), new_volume); //don't forget to assign new volume value to volume button
}
/**
 * Since each time the volume gets up by 8% from muted state we may end up to 96%(8*12)
 * so in that case 8% can't be added so just full volume state is signed 
 **/
if(volume >= 0.92) {
	g_object_set (play, "volume", 1.0 , NULL); 
	gtk_scale_button_set_value(GTK_SCALE_BUTTON(toolbar_item.volume), 1.0);
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.volume_up), FALSE); /* disable the volume up menu item */
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.volume_down), TRUE); /* enable the volume down menu item */
}

}	

/* volume down */
void volume_down_cb(GtkMenuItem *widget, gpointer data) {
gdouble volume = gtk_scale_button_get_value(GTK_SCALE_BUTTON(toolbar_item.volume));

gdouble new_volume = (volume - 0.08);

/* values under 0.0 and over 1.0 are not allowed */
if((new_volume > 0.0) && (new_volume < 1.0)) { 	
g_object_set (play, "volume", new_volume, NULL); 
gtk_scale_button_set_value(GTK_SCALE_BUTTON(toolbar_item.volume), new_volume); //don't forget to assign new volume value to volume button
}
/**
 * Since each time the volume gets reduced by 8% from 100% state we may end up to 4%
 * so in that case 8% can't be removed so just a muted state is signed
 **/
if(volume <= 0.08) {
	g_object_set (play, "volume", 0.0 , NULL); 
	gtk_scale_button_set_value(GTK_SCALE_BUTTON(toolbar_item.volume), 0.0);
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.volume_up), TRUE);    /* enable the volume up menu item */
    gtk_widget_set_sensitive(GTK_WIDGET(menu_item.volume_down), FALSE); /* disable the volume down menu item */
}
}

/* open from path dialog */
void open_from_path_cb(GtkMenuItem *widget, gpointer data) {
	
	GtkWidget *dialog;
	GtkWidget *table;
	GtkWidget *path_label;
	GtkWidget *path_entry;
	
	dialog = gtk_dialog_new_with_buttons("Open From Path...",
			GTK_WINDOW(gtk_widget_get_toplevel(widget)),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_MEDIA_PLAY, GTK_RESPONSE_OK,
			NULL);
	gtk_dialog_set_has_separator(GTK_DIALOG(dialog), FALSE);
	gtk_window_set_resizable(dialog, FALSE);

	GtkWidget *content_area = gtk_dialog_get_content_area(dialog);
	
	table = gtk_table_new(2, 2, FALSE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 8);
	gtk_table_set_col_spacings(GTK_TABLE(table), 8);
	gtk_container_set_border_width(GTK_CONTAINER(table), 8);

	gtk_box_pack_start(GTK_BOX(content_area), table, FALSE, FALSE, 0);

	/* find label */
	path_label = gtk_label_new_with_mnemonic("Enter Media Path:");
	gtk_misc_set_alignment(GTK_MISC(path_label), 0, 0.5);
	gtk_table_attach_defaults(GTK_TABLE(table), path_label, 0, 1, 0, 1);

	/* find entry */
	path_entry = gtk_entry_new();

	gtk_table_attach_defaults(GTK_TABLE(table), path_entry, 1, 2, 0, 1);
	gtk_label_set_mnemonic_widget(GTK_LABEL(path_label), path_entry);

	gtk_widget_show_all(table);
	
	GtkResponseType *response = gtk_dialog_run(GTK_DIALOG(dialog));
	
	/* if the ok button is clicked open the video file */
	  if (response == GTK_RESPONSE_OK) {
		  
    const gchar *uri = gtk_entry_get_text(GTK_ENTRY(path_entry));
		  
    gchar *name = g_path_get_basename(uri); /* getting the name of the file */
		  

		  GString *string  = g_string_new(name);
		  g_string_append(string, " - Tympanus Media Player");

		  gchar *title = g_string_free (string, FALSE);

		  gtk_window_set_title(GTK_WINDOW(window), title);
		  g_string_free(title, FALSE);

		  gtk_label_set_text(GTK_LABEL(filename_label), name);

		  play_pipeline(uri); //play the video by calling play_pipeline(char adress)
		  g_free(uri); //free the uri
		  
  }
    gtk_widget_destroy(dialog);
	
}	
/* open file dialog */

void open_file_cb(GtkWidget *widget, gpointer data) { 
	GtkWidget *open_dialog = gtk_file_chooser_dialog_new("Open Audio or Movie File...",
	                                                     gtk_widget_get_toplevel(GTK_WIDGET(widget)),
	                                                     GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                     GTK_STOCK_OPEN, GTK_RESPONSE_OK,
	                                                     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                     NULL);

	GtkResponseType *response = gtk_dialog_run(GTK_DIALOG(open_dialog));
	/* if the ok button is clicked open the video file */
	  if (response == GTK_RESPONSE_OK) {
		  
    gchar *uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(open_dialog));
	gchar *uri_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(open_dialog));	
		  
    gchar *name = g_path_get_basename(uri_name); /* getting the name of the file */
		  
		  
		  	gchar *string  = g_string_new(name);
	g_string_append(string, " - Tympanus Media Player");

	gchar *title = g_string_free (string, FALSE);
	
	gtk_window_set_title(GTK_WINDOW(window), title);
	g_string_free(title, FALSE);

	gtk_label_set_text(GTK_LABEL(filename_label), name);
		  
    gtk_widget_destroy(open_dialog);
		  if(play) { //if a video is already been played 
			  gst_element_set_state (play, GST_STATE_NULL); //destroy it first
		  }
		  play_pipeline(uri); //play the video by calling play_pipeline(char adress)
		    g_free(uri); //free the uri
		  
		  
  }
	/* if its not destroy the dialog */
 if (response == GTK_RESPONSE_CANCEL) {
    gtk_widget_destroy(open_dialog);
  }

	//gtk_widget_show_all(video_area);
}	

/* always on top dialog */
void always_on_top_cb(GtkCheckMenuItem *widget, gpointer data) {

	if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		
	gtk_window_set_modal(GTK_WINDOW(window), TRUE);
	gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
		
	}
	   else {

		   gtk_window_set_modal(GTK_WINDOW(window), FALSE);
		   gtk_window_set_keep_above(GTK_WINDOW(window), FALSE);

	   }
		   
}
	
/* about dialog */
void about_tympanus_cb(GtkMenuItem *widget, gpointer data) { 
	/* calling about_dialog(); */
	about_dialog();
}	


/* Creating the menubar */
 GtkWidget*
create_menubar(void) { 
	
	menubar = gtk_menu_bar_new();
	
	GtkWidget *file = gtk_menu_item_new_with_mnemonic("Movie");
	GtkWidget *file_menu = gtk_menu_new();

	/* open */
	GtkWidget *open = gtk_image_menu_item_new_with_mnemonic("Open...");
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(open), gtk_image_new_from_stock(GTK_STOCK_OPEN, GTK_ICON_SIZE_MENU));
	
	gtk_widget_add_accelerator(GTK_WIDGET(open), "activate", accel_group, 
    GDK_O, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
	g_signal_connect(open, "activate", G_CALLBACK(open_file_cb), NULL);

	/* open from path */
	GtkWidget *open_from_path = gtk_image_menu_item_new_with_mnemonic("Open From Path..."); 
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(open_from_path), gtk_image_new_from_stock(GTK_STOCK_DIRECTORY, GTK_ICON_SIZE_MENU));
	
	gtk_widget_add_accelerator(GTK_WIDGET(open_from_path), "activate", accel_group, 
    GDK_V, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
	g_signal_connect(open_from_path, "activate", G_CALLBACK(open_from_path_cb), NULL);
	
	GtkWidget *separator = gtk_separator_menu_item_new();

	/* properties */
	menu_item.properties = gtk_image_menu_item_new_with_mnemonic("Properties...");
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_item.properties), gtk_image_new_from_stock(GTK_STOCK_PROPERTIES, GTK_ICON_SIZE_MENU));
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.properties), FALSE);

	GtkWidget *separator2 = gtk_separator_menu_item_new();

	/* quit */
	GtkWidget *quit = gtk_image_menu_item_new_with_mnemonic("Quit");
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(quit), gtk_image_new_from_stock(GTK_STOCK_QUIT, GTK_ICON_SIZE_MENU));
	
	gtk_widget_add_accelerator(GTK_WIDGET(quit), "activate", accel_group, 
    GDK_Q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
	g_signal_connect(quit, "activate", G_CALLBACK(menubar_tympanus_quit), NULL);


	/* edit */
	GtkWidget *edit = gtk_menu_item_new_with_label("Edit");
	
	GtkWidget *edit_menu = gtk_menu_new();
	
	GtkWidget *fullscreen = gtk_check_menu_item_new_with_label("Fullscreen");
	GtkWidget *always_on_top = gtk_check_menu_item_new_with_label("Always On Top");
	g_signal_connect(always_on_top, "activate", G_CALLBACK(always_on_top_cb), NULL);
	
	menu_item.screenshot = gtk_image_menu_item_new_with_mnemonic("Take Screenshot...");
	gtk_widget_add_accelerator(GTK_WIDGET(menu_item.screenshot), "activate", accel_group, 
    GDK_KEY_S, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
	g_signal_connect(menu_item.screenshot, "activate", G_CALLBACK(screenshot_cb), NULL);
	
	/* view */
	GtkWidget *view = gtk_menu_item_new_with_mnemonic("View");
	GtkWidget *view_menu = gtk_menu_new();

	/* fullscreen */
	menu_item.fullscreen = gtk_check_menu_item_new_with_mnemonic("FullScreen Interface");
	
	gtk_widget_add_accelerator(menu_item.fullscreen, "activate", accel_group, 
    GDK_F11, 0, GTK_ACCEL_VISIBLE); 
	g_signal_connect(menu_item.fullscreen, "activate", G_CALLBACK(menu_fullscreen_interface_cb), NULL);
	
	GtkWidget *separator3 = gtk_separator_menu_item_new();
	
	GtkWidget *show_menubar = gtk_check_menu_item_new_with_mnemonic("Show Menubar");
	gtk_widget_add_accelerator(GTK_WIDGET(show_menubar), "activate", accel_group, 
    GDK_M, 0, GTK_ACCEL_VISIBLE); 
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(show_menubar), TRUE);
	g_signal_connect(show_menubar, "activate", G_CALLBACK(show_menubar_cb), NULL);

	GtkWidget *show_video_area = gtk_check_menu_item_new_with_mnemonic("Show Video Area");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(show_video_area), TRUE);
	gtk_widget_add_accelerator(GTK_WIDGET(show_video_area), "activate", accel_group, 
    GDK_V, 0, GTK_ACCEL_VISIBLE); 
	g_signal_connect(show_video_area, "activate", G_CALLBACK(hide_video_area_cb), NULL);
	
	GtkWidget *show_toolbar = gtk_check_menu_item_new_with_mnemonic("Show Controls");
	gtk_widget_add_accelerator(GTK_WIDGET(show_toolbar), "activate", accel_group, 
    GDK_C, 0, GTK_ACCEL_VISIBLE); 
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(show_toolbar), TRUE);
	g_signal_connect(show_toolbar, "activate", G_CALLBACK(show_toolbar_cb), NULL);
	/* make check buttons enabled by default */
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(show_menubar), TRUE);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(show_toolbar), TRUE);


    GtkWidget *show_statusbar = gtk_check_menu_item_new_with_mnemonic("Show Statusbar");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(show_statusbar), TRUE);
	gtk_widget_add_accelerator(GTK_WIDGET(show_statusbar), "activate", accel_group, 
    GDK_S, 0, GTK_ACCEL_VISIBLE); 
	g_signal_connect(show_statusbar, "activate", G_CALLBACK(show_statusbar_cb), NULL);

	/* TODO: Zoom In/Zoom Out/Zoom Normal */
	
	GtkWidget *sound = gtk_menu_item_new_with_mnemonic("Audio");
	GtkWidget *sound_menu = gtk_menu_new();
	menu_item.volume_up = gtk_menu_item_new_with_mnemonic("Volume Up");
	gtk_widget_add_accelerator(menu_item.volume_up, "activate", accel_group, 
    GDK_KEY_plus, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
	g_signal_connect(menu_item.volume_up, "activate", G_CALLBACK(volume_up_cb), NULL);
	
	menu_item.volume_down = gtk_menu_item_new_with_mnemonic("Volume Down");
	gtk_widget_add_accelerator(menu_item.volume_down, "activate", accel_group, 
    GDK_KEY_minus, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
	g_signal_connect(menu_item.volume_down, "activate", G_CALLBACK(volume_down_cb), NULL);
	
	GtkWidget *playback = gtk_menu_item_new_with_mnemonic("Playback");
	GtkWidget *playback_menu = gtk_menu_new();

	/* Play */
	menu_item.play = gtk_image_menu_item_new_with_mnemonic("Play");
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_item.play), gtk_image_new_from_stock(GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_MENU));
	
	gtk_widget_add_accelerator(menu_item.play, "activate", accel_group, 
    GDK_KEY_space, 0, GTK_ACCEL_VISIBLE); 
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.play), FALSE); //set disabled by default
	g_signal_connect(menu_item.play, "activate", G_CALLBACK(menu_pause_toggle_cb), NULL); 

	/* Pause */
	menu_item.pause = gtk_image_menu_item_new_with_mnemonic("Pause");
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_item.pause), gtk_image_new_from_stock(GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_MENU));
	
	gtk_widget_add_accelerator(menu_item.pause, "activate", accel_group, 
    GDK_KEY_space, 0, GTK_ACCEL_VISIBLE); 
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.pause), FALSE); //set disabled by default
	g_signal_connect(menu_item.pause, "activate", G_CALLBACK(menu_pause_toggle_cb), NULL); 
	
	GtkWidget *separator5 = gtk_separator_menu_item_new();

	/* Forward */
	menu_item.forward = gtk_image_menu_item_new_with_mnemonic("Forward");
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_item.forward), gtk_image_new_from_stock(GTK_STOCK_MEDIA_FORWARD, GTK_ICON_SIZE_MENU));
	gtk_widget_add_accelerator(GTK_WIDGET(menu_item.forward), "activate", accel_group, 
    GDK_KEY_rightarrow, 0, GTK_ACCEL_VISIBLE); 
	
	
	g_signal_connect(menu_item.forward, "activate", G_CALLBACK(menubar_forward_cb), NULL);

	/* rewind */
	menu_item.rewind = gtk_image_menu_item_new_with_mnemonic("Rewind");
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_item.rewind), gtk_image_new_from_stock(GTK_STOCK_MEDIA_REWIND, GTK_ICON_SIZE_MENU));
	gtk_widget_add_accelerator(GTK_WIDGET(menu_item.rewind), "activate", accel_group, 
    GDK_KEY_leftarrow, 0, GTK_ACCEL_VISIBLE); 
	
	g_signal_connect(menu_item.rewind, "activate", G_CALLBACK(menubar_rewind_cb), NULL);
		
	GtkWidget *separator6 = gtk_separator_menu_item_new();
	menu_item.faster = gtk_menu_item_new_with_mnemonic("Faster");
	g_signal_connect(menu_item.faster, "activate", G_CALLBACK(pipeline_faster_speed_cb), NULL);
	
	menu_item.speed_normal = gtk_menu_item_new_with_mnemonic("Normal Speed");
	g_signal_connect(menu_item.speed_normal, "activate", G_CALLBACK(pipeline_normal_speed_cb), NULL);

	menu_item.slower = gtk_menu_item_new_with_mnemonic("Slower");
	g_signal_connect(menu_item.slower, "activate", G_CALLBACK(pipeline_slower_speed_cb), NULL);
	
	GtkWidget *help = gtk_menu_item_new_with_mnemonic("Help");
	GtkWidget *help_menu = gtk_menu_new();

	
	/* about */
	GtkWidget *about = gtk_image_menu_item_new_with_mnemonic("About Tympanus");
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(about), gtk_image_new_from_stock(GTK_STOCK_ABOUT, GTK_ICON_SIZE_MENU));
	g_signal_connect(about, "activate", G_CALLBACK(about_tympanus_cb), NULL);


	//append items
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), GTK_WIDGET(file));
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), GTK_WIDGET(file_menu));
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), GTK_WIDGET(open));
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), GTK_WIDGET(open_from_path));
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), GTK_WIDGET(separator));
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), GTK_WIDGET(menu_item.properties));
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), GTK_WIDGET(separator2));
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), GTK_WIDGET(quit));

	
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), GTK_WIDGET(edit));
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), GTK_WIDGET(edit_menu));
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), GTK_WIDGET(fullscreen));
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), GTK_WIDGET(always_on_top));
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), GTK_WIDGET(menu_item.screenshot));

	
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), GTK_WIDGET(view));
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), GTK_WIDGET(view_menu));
	
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), GTK_WIDGET(menu_item.fullscreen));
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), GTK_WIDGET(separator3));
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), GTK_WIDGET(show_menubar));
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), GTK_WIDGET(show_video_area));
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), GTK_WIDGET(show_toolbar));
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), GTK_WIDGET(show_statusbar));
	
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), GTK_WIDGET(playback));
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(playback), GTK_WIDGET(playback_menu));
	                         
	gtk_menu_shell_append(GTK_MENU_SHELL(playback_menu), GTK_WIDGET(menu_item.play));
	gtk_menu_shell_append(GTK_MENU_SHELL(playback_menu), GTK_WIDGET(menu_item.pause));
	gtk_menu_shell_append(GTK_MENU_SHELL(playback_menu), GTK_WIDGET(separator5));
	gtk_menu_shell_append(GTK_MENU_SHELL(playback_menu), GTK_WIDGET(menu_item.rewind));
	gtk_menu_shell_append(GTK_MENU_SHELL(playback_menu), GTK_WIDGET(menu_item.forward));
	gtk_menu_shell_append(GTK_MENU_SHELL(playback_menu), GTK_WIDGET(separator6));
	gtk_menu_shell_append(GTK_MENU_SHELL(playback_menu), GTK_WIDGET(menu_item.faster));
	gtk_menu_shell_append(GTK_MENU_SHELL(playback_menu), GTK_WIDGET(menu_item.speed_normal));
	gtk_menu_shell_append(GTK_MENU_SHELL(playback_menu), GTK_WIDGET(menu_item.slower));
	
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), GTK_WIDGET(sound));
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(sound), GTK_WIDGET(sound_menu));
	gtk_menu_shell_append(GTK_MENU_SHELL(sound_menu), GTK_WIDGET(menu_item.volume_up));
	gtk_menu_shell_append(GTK_MENU_SHELL(sound_menu), GTK_WIDGET(menu_item.volume_down));
	
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), GTK_WIDGET(help));
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), GTK_WIDGET(help_menu));
	gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), GTK_WIDGET(about));

	/* disable menu items related to the pipeline */
	
	/* menu items */
	//gtk_widget_set_sensitive(GTK_WIDGET(menu_item.zoom_in), FALSE);
	//gtk_widget_set_sensitive(GTK_WIDGET(menu_item.zoom_normal), FALSE);
	//gtk_widget_set_sensitive(GTK_WIDGET(menu_item.zoom_out), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.volume_up), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.volume_down), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.forward), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.rewind), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.speed_normal), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.slower), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.faster), FALSE);
	
	return menubar;
}
