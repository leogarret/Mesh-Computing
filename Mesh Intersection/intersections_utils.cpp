#include "mc_vcg_intersection.hpp"
#include <math.h>

#define TOLERANCE 0.0001 // Tolérance de précision

/*
** Cette fonction retourne la distance entre deux points à trois dimensions
*/
inline double getPointsDistance(vcg::Point3d p1, vcg::Point3d p2)
{
	return sqrt((p1.X() - p2.X())*(p1.X() - p2.X()) + (p1.Y() - p2.Y())*(p1.Y() - p1.Y()) + (p1.Z() - p2.Z())*(p1.Z() - p1.Z()));
}

/*
** Cette fonction retourne la distance entre un point et une droite dans un espace à trois dimensions
*/
double getPointEdgeDistance(vcg::Point3d edge[2], vcg::Point3d p)
{
	double xa = edge[0].X();
	double ya = edge[0].Y();
	double za = edge[0].Z();
	/***************************/
	double xb = edge[1].X();
	double yb = edge[1].Y();
	double zb = edge[1].Z();
	/***************************/

	vcg::Point3d d = { xb - xa, yb - ya, zb - za }; // Vecteur directeur de 'edge'
	double dx = d.X();
	double dy = d.Y();
	double dz = d.Z();
	
	double dividende = (pow(dz*(p.Y() - ya) - dy*(p.Z() - za), 2) + pow(dx*(p.Z() - za) - dz*(p.X() - xa), 2) + pow(dy*(p.X() - xa) - dx*(p.Y() - ya), 2));
	double diviseur = pow(dx, 2)+pow(dy, 2)+pow(dz, 2);

	return dividende / diviseur; // distance
}

/*
** Cette fonction retourne le type d'objet sur lequel le paramètre 'points' se trouve.
** T_FACE s'il se trouve sur une face
** T_POINT s'il se trouve sur un sommet
** T_EDGE s'il se trouve sur une arête
*/
OBJTYPE onObjectType(MyFace face, vcg::Point3d points)
{
	/* Calcule des distances entre les points */
	double dist1 = getPointsDistance(face.P(0), points);

	double dist2 = getPointsDistance(face.P(1), points);

	double dist3 = getPointsDistance(face.P(2), points);

	/* 'points' se trouve-il sur un point ? */

	if (dist1 < TOLERANCE || dist2 < TOLERANCE || dist3 < TOLERANCE)
	{
		// 'points' se trouve sur un point (+/- TOLERANCE)
		return T_POINT;
	}

	/* Calcule des distances entre les points et arêtes */
	vcg::Point3d buff[2];
	buff[0] = face.cP0(0); buff[1] = face.cP0(1);
	dist1 = getPointEdgeDistance(buff, points);

	buff[0] = face.cP1(0); buff[1] = face.cP1(1);
	dist2 = getPointEdgeDistance(buff, points);

	buff[0] = face.cP2(0); buff[1] = face.cP2(1);
	dist3 = getPointEdgeDistance(buff, points);

	/* 'points' se trouve-il sur une arête ? */

	if (dist1 < TOLERANCE || dist2 < TOLERANCE || dist3 < TOLERANCE)
	{
		// 'points' se trouve sur un point (+/- TOLERANCE)
		return T_EDGE;
	}

	/* Si on est toujours dans la fonction, cela signifie que le point se trouve dans la face */
	return T_FACE;
}