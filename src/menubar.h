#ifndef MENUBAR_H
#define MENUBAR_H

/*
 * menubar.h
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

/* screenshot */
extern void screenshot_cb(GtkWidget *widget, gpointer data);

/* normal speed signal */
extern void pipeline_normal_speed_cb(GtkWidget *widget, gpointer data);

/* fast speed signal */
extern void pipeline_faster_speed_cb(GtkWidget *widget, gpointer data);

/* slow speed signal */
extern void pipeline_slower_speed_cb(GtkWidget *widget, gpointer data);
	
/* Quit */
extern void menubar_tympanus_quit(GtkMenuItem *widget, gpointer data);

/* view -> menubar */
extern void show_menubar_cb(GtkMenuItem *widget, gpointer data);

/* view -> controls */
extern void show_toolbar_cb(GtkMenuItem *widget, gpointer data); 

extern void show_statusbar_cb(GtkMenuItem *widget, gpointer data); 

/* show/hide video area */
extern void hide_video_area_cb(GtkMenuItem *widget, gpointer data);

/* toggle fullscreen interface */
extern void fullscreen_interface_cb(GtkMenuItem *widget, gpointer data);

/* toggle fullscreen interface FOR MENU ITEM*/
extern void menu_fullscreen_interface_cb(GtkMenuItem *widget, gpointer data);

/* rewind  */
extern void menubar_rewind_cb (GtkButton *widget, gpointer data);

/* forward */
extern void menubar_forward_cb (GtkMenuItem *widget, gpointer data);
	
/* volume up */
extern void volume_up_cb(GtkMenuItem *widget, gpointer data);

/* volume down */
extern void volume_down_cb(GtkMenuItem *widget, gpointer data);

/* open from path dialog */
extern void open_from_path_cb(GtkMenuItem *widget, gpointer data);

/* open file dialog */
extern void open_file_cb(GtkWidget *widget, gpointer data);

extern void about_tympanus_cb(GtkMenuItem *widget, gpointer data);

/* Creating the menubar */
extern GtkWidget* create_menubar(void);

#endif
