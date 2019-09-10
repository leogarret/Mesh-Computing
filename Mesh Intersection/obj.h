#ifndef _OBJ_H
#define _OBJ_H

/*!
* \file obj.h
* \brief Module pour le format obj
*/

#include "Mesh.h"

namespace obj {

	/*!
	*  \brief Charger un obj
	*
	*  Fonction qui permet de charger une forme 3D au format OBJ.
	*
	*  \param mesh : Le mesh qui va contenir la forme
	*  \param path : Le chemin d'accès de l'obj à charger
	*  \return Le temps de chargement en milliseconde si l'obj a été chargé.
	*  -1 si l'obj n'a pas chargé.
	*/
	long loader(Mesh &mesh, char *path, vcg::tri::io::ImporterOBJ<Mesh>::Info &mesh_info);
	void displayInfo(Mesh &mesh, vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info);

	namespace surface {

		long getPolygoneSurface(Mesh &mesh);

	}

};

#endif //!_OBJ_H