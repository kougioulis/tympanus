/*
 * toolbar.c
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

#include "toolbar.h"
#include "variables.h"
#include "signals.h"
#include "playback.h"

#define DURATION_IS_VALID(x) (x != 0 && x != (guint64) -1)

GtkWidget *toolbar;
GtkAdjustment *progress;


/* seek duration */
void
seek_cb (GtkRange *range,
                  GtkScrollType scroll,
                  gdouble value, gpointer data) {

	guint64 to_seek;

    if (!DURATION_IS_VALID (duration))
				 duration = backend_query_duration ();

    if (!DURATION_IS_VALID (duration)) {
      alert("Error", "Duration is not valid");
	}


    to_seek = (value / 100) * duration;

#if 0
    g_print ("value: %f\n", value);
    g_print ("duration: %llu\n", duration);
    g_print ("seek: %llu\n", to_seek);
#endif
    backend_seek_absolute (to_seek);
}

/* rewind  */
void
rewind_cb (GtkButton *rewind, gpointer data) {
			 
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
void
forward_cb (GtkWidget *widget, gpointer data) {
			 
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

/* toggle fullscreen */
void tool_fullscreen_cb(GtkButton *widget, gpointer data) {
	static gboolean fullscreen = FALSE;
	if(fullscreen) {
		  gtk_window_unfullscreen(GTK_WINDOW(window));
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_item.fullscreen),FALSE);
        fullscreen = FALSE;
    }
    else {
        gtk_window_fullscreen(GTK_WINDOW(window));
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_item.fullscreen),TRUE);
        fullscreen = TRUE;
    }
}

/* play/pause pipeline */
void pause_toggle_cb(GtkButton *widget, gpointer data) {
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

/* change volume */
gdouble change_volume_cb(GtkWidget *widget, gdouble value, gpointer data) {
g_object_set (play, "volume", value, NULL); 

if(value == 1.0) { 
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.volume_up), FALSE);    /* disable the volume up menu item */
    gtk_widget_set_sensitive(GTK_WIDGET(menu_item.volume_down), TRUE); /* true the volume down menu item */
}
else if ((value > 0.0) && (value < 1.0)) {
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.volume_up), TRUE);    /* enable both */
    gtk_widget_set_sensitive(GTK_WIDGET(menu_item.volume_down), TRUE); 
}
else if(value == 0.0) { 
	gtk_widget_set_sensitive(GTK_WIDGET(menu_item.volume_up), TRUE);    /* enable the volume up menu item */
    gtk_widget_set_sensitive(GTK_WIDGET(menu_item.volume_down), FALSE); /* disable the volume down menu item */

}
	return value;
}

/* stop pipeline */
void toolbar_stop_cb(GtkButton *button, gpointer data) {
	stop();
}

GtkWidget  *create_top_toolbar(void) { 
	GtkHBox *top_toolbar = gtk_hbox_new(FALSE, 0);

	//Time: Label
	toolbar_item.time_label = gtk_label_new("Time:");
	
	/* adjustment */
	progress = gtk_adjustment_new (0.00, 0.00, 100.00, 1.00, 0.00, 0.00);
    toolbar_item.hscale = gtk_hscale_new(progress);
    gtk_scale_set_draw_value(GTK_SCALE(toolbar_item.hscale), FALSE);	
	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.hscale), FALSE); //set unabled by default
   // gtk_widget_set_size_request(hscale, 200, NULL);
	 g_signal_connect (toolbar_item.hscale, "change-value", G_CALLBACK (seek_cb), NULL);
	//gtk_widget_set_can_focus(progress, FALSE);

	
	gtk_box_pack_start(GTK_BOX(top_toolbar), GTK_WIDGET(toolbar_item.time_label), FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(top_toolbar), GTK_WIDGET(toolbar_item.hscale), TRUE ,TRUE, 2);

	gtk_widget_show_all(GTK_WIDGET(top_toolbar));

	return top_toolbar;
}


/* creating the toolbar with the buttons */
 GtkWidget *create_toolbar(void)  {
	
	toolbar = gtk_hbox_new(FALSE, 0); //toolbar is in fact a horizontal box

	/* play */
	toolbar_item.play = gtk_button_new();
	gtk_button_set_image(GTK_BUTTON(toolbar_item.play), gtk_image_new_from_stock(GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_SMALL_TOOLBAR));
	gtk_button_set_relief(GTK_BUTTON(toolbar_item.play), GTK_RELIEF_NONE);
	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.play), FALSE); //set unabled by default
	gtk_widget_set_can_focus(GTK_WIDGET(toolbar_item.play), FALSE);

	/*
	 * the pause/play signal for the play button behaves like a toggle button
	 */
	g_signal_connect(toolbar_item.play, "clicked", G_CALLBACK(pause_toggle_cb), NULL);
	tooltips.play = gtk_tooltips_new();
	gtk_tooltips_set_tip(GTK_TOOLTIPS(tooltips.play), GTK_WIDGET(toolbar_item.play), "Play the current item", NULL);


	
	/* rewind */
	toolbar_item.rewind = gtk_button_new();
	gtk_button_set_image(GTK_BUTTON(toolbar_item.rewind), gtk_image_new_from_stock(GTK_STOCK_MEDIA_REWIND, GTK_ICON_SIZE_SMALL_TOOLBAR));
	gtk_button_set_relief(GTK_BUTTON(toolbar_item.rewind), GTK_RELIEF_NONE);
	tooltips.rewind= gtk_tooltips_new();
	gtk_tooltips_set_tip(GTK_TOOLTIPS(tooltips.rewind), GTK_WIDGET(toolbar_item.rewind), "Rewind", NULL);
	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.rewind), FALSE); //set unabled by default
	gtk_widget_set_can_focus(GTK_WIDGET(toolbar_item.rewind), FALSE);
	g_signal_connect(toolbar_item.rewind, "clicked", G_CALLBACK(rewind_cb), NULL);
	
	
	
	
	/* stop */
	toolbar_item.stop = gtk_button_new();
	gtk_button_set_image(GTK_BUTTON(toolbar_item.stop), gtk_image_new_from_stock(GTK_STOCK_MEDIA_STOP, GTK_ICON_SIZE_SMALL_TOOLBAR));
	gtk_button_set_relief(GTK_BUTTON(toolbar_item.stop), GTK_RELIEF_NONE);
	g_signal_connect(toolbar_item.stop, "clicked", G_CALLBACK(toolbar_stop_cb), NULL);
	tooltips.stop = gtk_tooltips_new();
	gtk_tooltips_set_tip(GTK_TOOLTIPS(tooltips.stop), GTK_WIDGET(toolbar_item.stop), "Stop playing the current item", NULL);
	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.stop), FALSE); //set unabled by default
	gtk_widget_set_can_focus(GTK_WIDGET(toolbar_item.stop), FALSE);

	/* fast forward */
	toolbar_item.forward = gtk_button_new();
	gtk_button_set_image(GTK_BUTTON(toolbar_item.forward), gtk_image_new_from_stock(GTK_STOCK_MEDIA_FORWARD, GTK_ICON_SIZE_SMALL_TOOLBAR));
	gtk_button_set_relief(GTK_BUTTON(toolbar_item.forward), GTK_RELIEF_NONE);
	tooltips.forward = gtk_tooltips_new();
	gtk_tooltips_set_tip(GTK_TOOLTIPS(tooltips.forward), GTK_WIDGET(toolbar_item.forward), "Forward", NULL);
	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.forward), FALSE); //set unabled by default
	gtk_widget_set_can_focus(GTK_WIDGET(toolbar_item.forward), FALSE);
	g_signal_connect(toolbar_item.forward, "clicked", G_CALLBACK(forward_cb), NULL);


	//Time: Label
	toolbar_item.time_label = gtk_label_new("Time:");
	
	/* adjustment */
	progress = gtk_adjustment_new (0.00, 0.00, 100.00, 1.00, 0.00, 0.00);
    toolbar_item.hscale = gtk_hscale_new(progress);
    gtk_scale_set_draw_value(GTK_SCALE(toolbar_item.hscale), FALSE);	
	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.hscale), FALSE); //set unabled by default
   // gtk_widget_set_size_request(hscale, 200, NULL);
	 g_signal_connect (toolbar_item.hscale, "change-value", G_CALLBACK(seek_cb), NULL);
	//gtk_widget_set_can_focus(progress, FALSE);

	 
	/* fullscreen */
	toolbar_item.fullscreen = gtk_button_new();
	gtk_button_set_image(GTK_BUTTON(toolbar_item.fullscreen), gtk_image_new_from_stock(GTK_STOCK_FULLSCREEN, GTK_ICON_SIZE_SMALL_TOOLBAR));
	gtk_button_set_relief(GTK_BUTTON(toolbar_item.fullscreen), GTK_RELIEF_NONE);
	g_signal_connect(toolbar_item.fullscreen, "clicked", G_CALLBACK(tool_fullscreen_cb), NULL);
	tooltips.fullscreen = gtk_tooltips_new();
	gtk_tooltips_set_tip(GTK_TOOLTIPS(tooltips.fullscreen), GTK_WIDGET(toolbar_item.fullscreen), "View item fullscreen", NULL);
	gtk_widget_set_can_focus(GTK_WIDGET(toolbar_item.fullscreen), FALSE);



	 /* TODO: Elapsed time */
	//Elapsed time label
	//toolbar_item.elapsed_label  =gtk_label_new("0:00");
	//toolbar_item.total_time_label = gtk_label_new("/0:00");
	
	/* volume */
	toolbar_item.volume = gtk_volume_button_new();
	 gtk_button_set_relief(GTK_BUTTON(toolbar_item.volume), GTK_RELIEF_NONE);	
	gtk_scale_button_set_value(GTK_SCALE_BUTTON(toolbar_item.volume), 0.00); //set as muted by default
	g_signal_connect(toolbar_item.volume, "value-changed", G_CALLBACK(change_volume_cb), NULL);
	gtk_widget_set_sensitive(GTK_WIDGET(toolbar_item.volume), FALSE); //set unabled by default
	gtk_widget_set_can_focus(GTK_WIDGET(toolbar_item.volume), FALSE);


	
	/* Append all the items to the toolbar */
	gtk_box_pack_start(GTK_BOX(toolbar), GTK_WIDGET(toolbar_item.play), FALSE ,FALSE, 0);
	gtk_box_pack_start(GTK_BOX(toolbar), GTK_WIDGET(toolbar_item.rewind), FALSE ,FALSE, 2);
	gtk_box_pack_start(GTK_BOX(toolbar), GTK_WIDGET(toolbar_item.stop), FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(toolbar), GTK_WIDGET(toolbar_item.forward), FALSE, FALSE, 2);

	gtk_box_pack_start(GTK_BOX(toolbar), GTK_WIDGET(toolbar_item.time_label), FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(toolbar), GTK_WIDGET(toolbar_item.hscale), TRUE ,TRUE, 2);

	gtk_box_pack_end(GTK_BOX(toolbar), GTK_WIDGET(toolbar_item.fullscreen), FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(toolbar), GTK_WIDGET(toolbar_item.volume), FALSE, FALSE, 3);
	//gtk_box_pack_start(toolbar, toolbar_item.time_label, FALSE, FALSE, 1);
	//gtk_box_pack_start(toolbar, toolbar_item.hscale, TRUE ,TRUE, 2);
	//gtk_box_pack_start(toolbar, toolbar_item.elapsed_label, FALSE, FALSE, 0);
	//gtk_box_pack_start(toolbar, toolbar_item.total_time_label, FALSE, FALSE, 1);

	
	return toolbar;
}

