#ifndef VARIABLES_H
#define VARIABLES_H

/*
 * variables.h
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
#include <gst/gst.h>
#define DURATION_IS_VALID(x) (x != 0 && x != (guint64) -1)

extern double speed_value;
extern double normal_value;
extern double default_value;

extern guint64 duration;
extern GtkWidget *menubar;
extern GtkWidget *window;
extern GtkWidget *toolbar;
extern GtkWidget *statusbar;
extern GtkWidget *separator_hbox;
extern GtkWidget *preferences_vbox;

extern GtkAdjustment *progress;
extern GstElement *pipeline;
extern GtkWidget *video_area;
extern cairo_t *cr;
extern GstElement *play;
extern GstElement *src, *sink;
extern GtkAccelGroup *accel_group;
extern GstElement *x_overlay;

/* menu buttons */
struct menu_item_struct {
	GtkWidget *properties;
	GtkWidget *play;
	GtkWidget *pause;
	GtkWidget *zoom_in;
	GtkWidget *zoom_normal;
	GtkWidget *zoom_out;
	GtkWidget *volume_up;
	GtkWidget *volume_down;
	GtkWidget *forward;
	GtkWidget *rewind;
	GtkWidget *speed_normal;
	GtkWidget *slower;
	GtkWidget *faster;
	GtkWidget *fullscreen;
	GtkWidget *screenshot;
} menu_item;

extern struct menu_item_struct menu_item;

/* toolbar buttons */
struct toolbar_item_struct {
	GtkWidget *play;
	GtkWidget *rewind;
	GtkWidget *stop;
	GtkWidget *forward;
	GtkWidget *reset;
	GtkWidget *fullscreen;
	GtkWidget *hscale;
	GtkWidget *time_label;
	GtkWidget *elapsed_label;
	GtkWidget *total_time_label;
	GtkWidget *volume;
} toolbar_item;

extern struct toolbar_item_struct toolbar_item;

/* TODO: MENU ITEMS STRUCT */
	/*Tooltips  */
struct tooltips_struct {
	GtkTooltip *play;
	GtkTooltip *rewind;
	GtkTooltip *stop;
	GtkTooltip *forward;
	GtkTooltip *fullscreen;
	GtkTooltip *time_label;
	GtkTooltip *elapsed_time_label;
} tooltips;

extern struct tooltips_struct tooltips;

extern GtkWidget *status_label;
extern GtkWidget *status_label_sep;
extern GtkWidget *filename_label;
extern GtkWidget *filename_label_sep;
extern GtkWidget *speed_label;
extern GtkWidget *speed_label_sep;

extern GstSeekFlags seek_flags;

/* right menu items (video_area.c) */

extern GtkWidget *right_play_pause;
extern GtkWidget *right_rewind;
extern GtkWidget *right_forward;
extern GtkWidget *right_fullscreen;

#endif
