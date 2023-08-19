/*
 * about_dialog.c
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

#include "about_dialog.h"
#include "config.h"

/* about dialog */
void about_dialog(void) { 
	/* no need to create an external function named about_tympanus() 
	 * because this callback is only called by the "about" menu item
	 */

	GtkWidget *about_dialog = gtk_about_dialog_new();

	/* authors array */
	 static const gchar *authors[] = {
		"Nikolaos Kougioulis <nkougioul@hotmail.com>",
		NULL
	};

	/* copyright */
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(about_dialog), "Copyright (c) Nikolaos Kougioulis");
	
	/* comments */
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about_dialog), "A Lightweight yet powerful media player");
	
	/* name */
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(about_dialog), "Tympanus Media Player");
	
	/* logo */
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about_dialog), gdk_pixbuf_new_from_file("src/images/icons/video64.png", NULL));
	
	/* version */
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about_dialog), "1.0.3");
	
	/* wrap license */                                                                                  
	gtk_about_dialog_set_wrap_license(GTK_ABOUT_DIALOG(about_dialog), TRUE);
	
	/* positioning at the center */
	gtk_window_set_position(GTK_WINDOW(about_dialog), GTK_WIN_POS_CENTER);
	
	/* authors */
	gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(about_dialog), authors);
	gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(about_dialog), authors);
	
	/* license */
	gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(about_dialog),
    "Tympanus is released under the GNU General Public Software License. \n" 
    "You are free to distribute it and/or modify it under the terms \n"
    "of the Public Software License. \n"
	"\n"
	"You may not use this program unless in compliance with the terms "
    "stated in the license. You should have recieved a copy of the GNU General Public "
    "Software License. If not, see <http://www.gnu.org/licenses>.");
	                               
	/* run the dialog */
	gtk_dialog_run(GTK_DIALOG(about_dialog));
	/* dialog will be destroyed by the press of the "OK" button */
	gtk_widget_destroy(about_dialog);

}	


