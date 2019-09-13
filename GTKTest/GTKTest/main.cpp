#ifndef _GRAPH_UI_C
#define _GRAPH_UI_C

#include <iostream>
#include <ui.h>
#include <obj.h>
#include <pango\pango.h>

#endif // !_GRAPH_UI_C

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);
	GtkWidget *window = ui::window::createWindowFromBuilder("window.ui", "window");

	Mesh mesh;
	vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info;

	if (obj::loader(mesh, "../../../obj/cube.obj", mesh_info) == -1)
		return -1;

	ui::MeshDisplayInfo(GTK_WINDOW(window), mesh, mesh_info);



	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}