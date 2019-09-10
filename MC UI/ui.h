#ifndef _UI_H
#define _UI_H

#include <iostream>
#include <gtk-2.0\gtk\gtk.h>

namespace ui {

	void start_graphics(int ac, char **av);

	namespace window {

		/*!
		*  \brief Cr�er une fen�tre principale � partir d'un builder.
		*
		*  Fonction qui permet de cr�er une fen�tre principale � partir
		*  d'un builder XML qui peut �tre g�n�r� avec Glade.
		*
		*  \param builderPath : chemin d'acc�s du fichier XML.
		*  \param windowComponentName : nom du composant de la fen�tre dans le fichier XML.
		*  \return Le widget contenant les propri�t�s de la fen�tre.
		*/
		GtkWidget *createWindowFromBuilder(char *builderPath, char *windowComponentName);

	};

};

#endif // !_UI_H