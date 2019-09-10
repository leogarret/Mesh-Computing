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
	/*!
	*  \brief Afficher les informations d'un mesh.
	*
	*  Fonction qui permet d'afficher les informations d'un mesh
	*  au format obj : nombres de sommets, de faces, d'arêtes et
	*  l'air du polygone.
	*
	*  \param mesh : Le mesh sur lequel nous voulons les informations.
	*  \param path : Le mesh_info du mesh en question.
	*/
	void displayInfo(Mesh &mesh, vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info);

	namespace surface {
		/*!
		*  \brief Obtenir l'air total de la surface d'un mesh.
		*
		*  Fonction qui permet d'obtenir l'air total de la surface 
		*  d'un mesh au format obj.
		*
		*  \param mesh : Le mesh sur lequel nous voulons récupérer l'air.
		*
		*  \return L'air du mesh dans un long.
		*/
		long getPolygoneSurface(Mesh &mesh);

	}

};

#endif //!_OBJ_H