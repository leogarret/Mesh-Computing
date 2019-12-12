#include "mc_vcg_intersection.hpp"
#include <math.h>
using namespace mc;

#define TOLERANCE 0.0001 // Tolérance de précision

/*
** Retourne la projection d'un point 'p' sur une droite 'edge'.
*/
vcg::Point3d getProjection(vcg::Point3d edge[2], vcg::Point3d p)
{
	double xa = edge[0].X();
	double ya = edge[0].Y();
	double za = edge[0].Z();
	/***************************/
	double xb = edge[1].X();
	double yb = edge[1].Y();
	double zb = edge[1].Z();
	/***************************/

	vcg::Point3d vDir = { xb - xa, yb - ya, zb - za }; // Vecteur directeur de 'edge'
	double dx = vDir.X();
	double dy = vDir.Y();
	double dz = vDir.Z();

	double den = dx*dx + dy*dy + dz*dz;
	if (den == 0)
		return NULL;

	double d = -vDir.dot(p);
	double rho = (dx * xa + dy * ya + dz * za + d) / den;

	vcg::Point3d proj(xa - dx * rho, ya - dy * rho, za - dz * rho);

	return proj;
}

/*
** Cette fonction retourne la distance entre deux points à trois dimensions
*/
double getPointsDistance(vcg::Point3d p1, vcg::Point3d p2)
{
	double xDist = pow(p1.X() - p2.X(), 2);
	double yDist = pow(p1.Y() - p2.Y(), 2);
	double zDist = pow(p1.Z() - p2.Z(), 2);

	return sqrt(xDist + yDist + zDist);
}

double geom::getPointEdgeDistance(vcg::Point3d edge[2], vcg::Point3d p)
{
	double xa = edge[0].X();
	double ya = edge[0].Y();
	double za = edge[0].Z();
	/***************************/
	double xb = edge[1].X();
	double yb = edge[1].Y();
	double zb = edge[1].Z();
	/***************************/

	vcg::Point3d vDir = { xb - xa, yb - ya, zb - za }; // Vecteur directeur de 'edge'
	double dx = vDir.X();
	double dy = vDir.Y();
	double dz = vDir.Z();
	
	vcg::Point3d proj(getProjection(edge, p)); // On récupère la projection de 'p' sur 'edge'
	
	return getPointsDistance(p, proj);
}

/*
** Cette fonction retourne le type d'objet sur lequel le paramètre 'points' se trouve.
** T_FACE s'il se trouve sur une face
** T_POINT s'il se trouve sur un sommet
** T_EDGE s'il se trouve sur une arête
*/
intersection::OBJTYPE intersection::onObjectType(MCFace face, vcg::Point3d points)
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
	dist1 = geom::getPointEdgeDistance(buff, points);

	buff[0] = face.cP1(0); buff[1] = face.cP1(1);
	dist2 = geom::getPointEdgeDistance(buff, points);

	buff[0] = face.cP2(0); buff[1] = face.cP2(1);
	dist3 = geom::getPointEdgeDistance(buff, points);

	/* 'points' se trouve-il sur une arête ? */

	if (dist1 < TOLERANCE || dist2 < TOLERANCE || dist3 < TOLERANCE)
	{
		// 'points' se trouve sur un point (+/- TOLERANCE)
		return T_EDGE;
	}

	/* Si on est toujours dans la fonction, cela signifie que le point se trouve dans la face */
	return T_FACE;
}