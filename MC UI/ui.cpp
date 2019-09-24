#ifndef _UI_C
#define _UI_C

#include "ui.h"

#endif // !_UI_C

void ui::MeshDisplayInfo(GtkLabel *label, mc::mvcg::Mesh &mesh, vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::Info mesh_info)
{
	std::string mesh_info_ui;
	char buff[100];

	mesh_info_ui.append("<b><u>Mesh Informations</u>\n\nNumber of faces: </b>");
	_itoa_s(mesh_info.numFaces, buff, 10);
	mesh_info_ui.append(buff);
	mesh_info_ui.append("\n<b>Number edges: </b>");
	_itoa_s(mesh_info.numEdges, buff, 10);
	mesh_info_ui.append(buff);
	mesh_info_ui.append("\n<b>Number of vertices: </b>");
	_itoa_s(mesh_info.numVertices, buff, 10);
	mesh_info_ui.append(buff);
	mesh_info_ui.append("\n<b>Surface: </b>");
	mesh_info_ui.append(buff);

	gtk_label_set_text(GTK_LABEL(label), mesh_info_ui.c_str());

	gtk_label_set_use_markup(label, TRUE);
}