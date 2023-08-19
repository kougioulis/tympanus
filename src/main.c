/*
 * main.c
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

#include <stdlib.h> 
#include <stdio.h>
#include <gst/interfaces/xoverlay.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <gst/gst.h>
#include <gdk/gdkkeysyms.h>


#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h> 
#endif 

/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

//#include "variables.h"
#include "menubar.h"
#include "signals.h"
#include "video_area.h"
#include "toolbar.h"
#include "statusbar_separator.h"
#include "statusbar.h"
#include "window.h"

/* For testing propose use the local (not installed) ui file */
#define UI_FILE "src/tympanus.ui"

#define DURATION_IS_VALID(x) (x != 0 && x != (guint64) -1)

int main (int argc, char *argv[]) {
	GtkWidget *vbox;

	
	gtk_init (&argc, &argv); //init gtk
	gst_init(&argc, &argv); //int gstreamer

	GtkWidget *window = create_window ();
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);	
	
	GtkWidget *menubar = create_menubar();
	GtkWidget *video_area = create_video_area();
//	GtkWidget *top_toolbar = create_top_toolbar();
	GtkWidget *toolbar = create_toolbar();
	GtkWidget *statusbar_separator = create_statusbar_separator();
	GtkWidget *statusbar = create_statusbar();
	
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), video_area, TRUE, TRUE, 0);

	//gtk_box_pack_start(vbox, top_toolbar, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(vbox), statusbar_separator, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, FALSE, 0);

	
	gtk_widget_show_all(vbox);
	gtk_widget_show_all(window);

	gtk_main();
	
	return 0;
}
