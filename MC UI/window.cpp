#ifndef _WINDOW_C
#define _WINDOW_C

#include "ui.h"

#endif // !_WINDOW_C

GtkWidget *ui::window::createWindowFromBuilder(char *builderPath, char *windowComponentName)
{
	GtkBuilder *builder = gtk_builder_new();
	char *filename = g_build_filename(builderPath, NULL);

	GError *error = NULL;
	gtk_builder_add_from_file(builder, filename, &error);
	g_free(error);

	if (error) {
		gint code = error->code;
		g_printerr("%s\n", error->message);
		g_error_free(error);
		exit(-1);
	}

	GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, windowComponentName));
	g_signal_connect(G_OBJECT(window), "destroy", (GCallback)gtk_main_quit, NULL);

	return window;
}