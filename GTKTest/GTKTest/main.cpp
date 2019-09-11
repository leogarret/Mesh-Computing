#ifndef _GRAPH_UI_C
#define _GRAPH_UI_C

#include <iostream>
#include <gtk/gtk.h>
#include <ui.h>

#endif // !_GRAPH_UI_C

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);
	GtkWidget *window = ui::window::createWindowFromBuilder("window.ui", "window");

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}