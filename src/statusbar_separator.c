/*
 * statusbar_separator.c
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

#include "statusbar_separator.h"
#include "variables.h"

GtkWidget *separator_hbox;

/* Statusbar */
GtkWidget* create_statusbar_separator(void)  {

	
	separator_hbox = gtk_hbox_new(FALSE, 0);
	GtkHSeparator *statusbar_separator = gtk_hseparator_new();

	gtk_box_pack_start(GTK_BOX(separator_hbox), GTK_WIDGET(statusbar_separator), TRUE, TRUE, 0);

	gtk_widget_show_all(separator_hbox);

	return separator_hbox;
}		
