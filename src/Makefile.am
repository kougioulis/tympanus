## Process this file with automake to produce Makefile.in



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(TYMPANUS_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = tympanus

tympanus_SOURCES = \
        lang.h \
        playback.c \
        playback.h \
        signals.c \
        signals.h \
        menubar.c \
        menubar.h \
        statusbar.c \
        statusbar.h \
        statusbar_separator.c \
        statusbar_separator.h \
        toolbar.c \
        toolbar.h \
        variables.h \
        video_area.h \
        video_area.c \
        window.h \
        window.c \
        about_dialog.h  \
        about_dialog.c \
	main.c

tympanus_LDFLAGS = \
	-Wl,--export-dynamic

tympanus_LDADD = $(TYMPANUS_LIBS)


