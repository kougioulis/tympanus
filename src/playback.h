#ifndef PLAYBACK_H
#define PLAYBACK_H
#define DURATION_IS_VALID(x) (x != 0 && x != (guint64) -1)

/*
 * playback.h
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

/* method to display an alert message */
extern void alert(char *error_title, char *error_message);
	
/* play */
extern void play_pipeline(gchar *addr);

/* stop */
extern void stop(void);

/* reset */
extern void reset(void);

/* helpers for seek_cb */
extern void backend_seek_absolute (guint64 value);

guint64 backend_query_position (void);

guint64 backend_query_duration (void);

#endif
