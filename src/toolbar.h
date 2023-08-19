#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <gtk/gtk.h>
#include "variables.h"

/*
 * toolbar.h
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


/* seek duration */
extern void
seek_cb (GtkRange *range,
                  GtkScrollType scroll,
                  gdouble value, gpointer data);

/* rewind  */
extern void
rewind_cb (GtkButton *rewind, gpointer data);

/* forward */
extern void
forward_cb (GtkWidget *widget, gpointer data);

/* toggle fullscreen */
extern void tool_fullscreen_cb(GtkButton *widget, gpointer data);


/* play/pause pipeline */
extern void pause_toggle_cb(GtkButton *widget, gpointer data);


/* change volume */
extern gdouble change_volume_cb(GtkWidget *widget, gdouble value, gpointer data);


/* stop pipeline */
extern void toolbar_stop_cb(GtkButton *button, gpointer data);


extern GtkWidget *create_top_toolbar(void);


/* creating the toolbar with the buttons */
extern GtkWidget *create_toolbar(void);

#endif
