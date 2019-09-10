#ifndef _UI_H
#define _UI_H

#include <iostream>
#include <gtk-2.0\gtk\gtk.h>

namespace ui {

	void start_graphics(int ac, char **av);

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