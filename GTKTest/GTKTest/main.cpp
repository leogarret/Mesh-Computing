#ifndef _GRAPH_UI_C
#define _GRAPH_UI_C

#include <iostream>
#include <ui.h>

#endif // !_GRAPH_UI_C

SGlobalData data;

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);

	GtkWidget *window = ui::window::createWindowFromBuilder(&data, "window");

	gtk_widget_modify_font(GTK_WIDGET(gtk_builder_get_object(data.builder, "meshInfoLbl")), pango_font_description_from_string("Arial 15"));

	g_signal_connect(gtk_builder_get_object(data.builder, "OpenFileSubMenuItem"), "button-press-event", G_CALLBACK(ui::callback_open_file_dialog), NULL);

	gtk_widget_show_all(window);

	gtk_main();
	return 0;
}