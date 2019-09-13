#ifndef _UI_C
#define _UI_C

#include "ui.h"

#endif // !_UI_C

void ui::MeshDisplayInfo(GtkWindow *window, Mesh &mesh, vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info)
{
	std::string mesh_info_ui;
	char buff[10];

	mesh_info_ui.append("<b><u>Mesh Informations</u>\n\nNumber of faces: </b>");
	_itoa_s(mesh_info.numFaces, buff, 10);
	mesh_info_ui.append(buff);
	mesh_info_ui.append("\n<b>Number edge: </b>");
	_itoa_s(mesh_info.numEdges, buff, 10);
	mesh_info_ui.append(buff);
	mesh_info_ui.append("\n<b>Number of vertices: </b>");
	_itoa_s(mesh_info.numVertices, buff, 10);
	mesh_info_ui.append(buff);
	mesh_info_ui.append("\n<b>Surface: </b>");
	_itoa_s(obj::surface::getPolygoneSurface(mesh), buff, 10);
	mesh_info_ui.append(buff);

	GtkWidget* label = gtk_label_new(mesh_info_ui.c_str());
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_widget_modify_font(label, pango_font_description_from_string("Arial 20"));

	gtk_container_add(GTK_CONTAINER(window), label);
}