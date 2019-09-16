#ifndef _UI_H
#define _UI_H

#include <iostream>
#include <gtk\gtk.h>
#include <obj.h>
#include <pango\pango.h>

typedef struct
{
	GtkBuilder *builder;
	gpointer user_data;
} SGlobalData;

extern SGlobalData data;


namespace ui {

	void MeshDisplayInfo(GtkLabel *label, Mesh &mesh, vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info);

	void callback_open_file_dialog();

	namespace window {

		/*!
		*  \brief Cr�er une fen�tre principale � partir d'un builder.
		*
		*  Fonction qui permet de cr�er une fen�tre principale � partir
		*  d'un builder XML qui peut �tre g�n�r� avec Glade.
		*
		*  \param builder : Adresse du builder non initialis�.
		*  \param windowComponentName : nom du composant de la fen�tre dans le fichier XML.
		*  \return Le widget contenant les propri�t�s de la fen�tre.
		*/
		GtkWidget *createWindowFromBuilder(SGlobalData *data, char *windowComponentName);
	};

};

#endif // !_UI_H