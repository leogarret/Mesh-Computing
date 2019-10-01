//#ifndef _UI_H
//#define _UI_H
//
//#include <iostream>
//#include <windows.h>
//
//#include <gtk\gtk.h>
//#include <pango\pango.h>
//
//#include <mc_vcg_mesh.hpp>
//
//typedef struct
//{
//	GtkBuilder *builder;
//	gpointer user_data;
//} SGlobalData;
//
//extern SGlobalData data;
//
//
//namespace ui {
//
//	void MeshDisplayInfo(GtkLabel *label, mc::mvcg::Mesh &mesh, vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::Info mesh_info);
//
//	void callback_open_file_dialog();
//
//	namespace window {
//
//		/*!
//		*  \brief Créer une fenêtre principale à partir d'un builder.
//		*
//		*  Fonction qui permet de créer une fenêtre principale à partir
//		*  d'un builder XML qui peut être généré avec Glade.
//		*
//		*  \param builder : Adresse du builder non initialisé.
//		*  \param windowComponentName : nom du composant de la fenêtre dans le fichier XML.
//		*  \return Le widget contenant les propriétés de la fenêtre.
//		*/
//		GtkWidget *createWindowFromBuilder(SGlobalData *data, char *windowComponentName);
//	};
//
//};
//
//#endif // !_UI_H