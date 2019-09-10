#ifndef _UI_C
#define _UI_C

#include "ui.h"

#endif // !_UI_C

void ui::start_graphics(int ac, char **av)
{
	gtk_init(&ac, &av);
	GtkWidget *window = ui::window::createWindowFromBuilder("test.glade", "window");

	gtk_widget_show_all(window);
	gtk_main();
}