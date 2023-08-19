/*
 * statusbar.c
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
#include "statusbar.h"

GtkWidget *statusbar;
GtkWidget *status_label;
GtkWidget *status_label_sep;

GtkWidget *filename_label;
GtkWidget *filename_label_sep;

GtkWidget *speed_label;
GtkWidget *speed_label_sep; 

/* Statusbar */
GtkWidget*
create_statusbar(void)  {
	
	statusbar = gtk_hbox_new(FALSE, 0);

	status_label = gtk_label_new("Ready");
	status_label_sep = gtk_separator_tool_item_new();
	
	filename_label = gtk_label_new("No media selected");
	filename_label_sep = gtk_separator_tool_item_new();

	speed_label = gtk_label_new("1.00x");
	speed_label_sep = gtk_separator_tool_item_new();

	gtk_box_pack_start(GTK_BOX(statusbar), GTK_WIDGET(status_label), FALSE, FALSE, 4);
	gtk_box_pack_start(GTK_BOX(statusbar), GTK_WIDGET(status_label_sep), FALSE, FALSE, 0);
	
	gtk_box_pack_start(GTK_BOX(statusbar), GTK_WIDGET(filename_label), FALSE, FALSE, 4);
	gtk_box_pack_start(GTK_BOX(statusbar), GTK_WIDGET(filename_label_sep), FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(statusbar), GTK_WIDGET(speed_label), FALSE, FALSE, 4);
	gtk_box_pack_start(GTK_BOX(statusbar), GTK_WIDGET(speed_label_sep), FALSE, FALSE, 0);

	return statusbar;
}	
