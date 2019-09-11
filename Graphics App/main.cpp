#ifndef _G_MAIN_C
#define _G_MAIN_C

#include <iostream>
#include <obj.h>
#include <ui.h>

#include <gtk-2.0\gtk\gtk.h>

#endif // !_G_MAIN_C

int main(int argc, char* argv[])
{
	gtk_init(&argc, &argv);

	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(window, 300, 200);

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_title(GTK_WINDOW(window), "GTK+ with VS2010");

	gtk_widget_show(window);

	gtk_main();
	return 0;
}