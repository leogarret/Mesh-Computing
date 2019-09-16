#ifndef _WINDOW_C
#define _WINDOW_C

#include "ui.h"

#endif // !_WINDOW_C

GtkWidget *ui::window::createWindowFromBuilder(SGlobalData *data, char *windowComponentName)
{
	data->builder = gtk_builder_new();
	GError *buffError = NULL;
	gchar *filename = g_build_filename("window.ui", NULL);


	gtk_builder_add_from_file(data->builder, filename, &buffError);

	if (buffError) {
		gint code = buffError->code;
		g_printerr("%s\n", buffError->message);
		g_error_free(buffError);
		exit(-1);
	}

	GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(data->builder, windowComponentName));
	gtk_builder_connect_signals(data->builder, data);
	return window;
}