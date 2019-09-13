#ifndef _UI_H
#define _UI_H

#include <iostream>
#include <gtk\gtk.h>
#include <obj.h>

namespace ui {

	void MeshDisplayInfo(GtkWindow *window, Mesh &mesh, vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info);

	namespace window {

		/*!
		*  \brief Créer une fenêtre principale à partir d'un builder.
		*
		*  Fonction qui permet de créer une fenêtre principale à partir
		*  d'un builder XML qui peut être généré avec Glade.
		*
		*  \param builderPath : chemin d'accès du fichier XML.
		*  \param windowComponentName : nom du composant de la fenêtre dans le fichier XML.
		*  \return Le widget contenant les propriétés de la fenêtre.
		*/
		GtkWidget *createWindowFromBuilder(char *builderPath, char *windowComponentName);

	};

};

#endif // !_UI_H