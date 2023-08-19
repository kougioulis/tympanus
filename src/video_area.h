#ifndef VIDEO_AREA_H
#define VIDEO_AREA_H

/*
 * video_area.h
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

#include <gtk/gtk.h>
#include "variables.h"

GtkWidget *video_area;
GtkWidget *window;


extern void show_menu_cb(GtkWidget *widget, gpointer data);

extern int video_area_toggle_cb(GtkWidget *video_area, GdkEventButton *event, gpointer data);

extern gboolean
video_area_expose(GtkWidget *video_area, GdkEvent *event, gpointer data);

extern GtkWidget* create_video_area(void);

#endif
