#ifndef _START_GRAPHICS_C
#define _START_GRAPHICS_C

#include <iostream>
#include 

#endif // !_START_GRAPHICS_C

int main(int argc, char **argv)
{
	ui::start_graphics(argc, argv);
	return 0;
}

void ui::start_graphics(int ac, char **av)
{
	gtk_init(&ac, &av);
	GtkWidget *window = ui::window::createWindowFromBuilder("test.glade", "window");

	gtk_widget_show_all(window);
	gtk_main();
}