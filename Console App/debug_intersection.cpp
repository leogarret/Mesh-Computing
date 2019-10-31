#include <mc_vcg_intersection.hpp>
#include <mc_vcg_obj_importer.hpp>
#include <wrap\io_trimesh\export_obj.h>
#include <ctime>
#include <logger.hpp>

#undef max;

typedef double MScalarType;
typedef vcg::AABBBinaryTreeIndex<MyFace, MScalarType, vcg::EmptyClass> MIndex;

typedef struct BuffIntersect {
	int nbIntersections;
	std::vector<MyFace*> facesIntersections;
	std::vector<MIndex::CoordType> pointsIntersections;
	vcg::Point3d origin;
	vcg::Point3d direction;
	std::clock_t time;
} BuffIntersect;

BuffIntersect initBuffIntersect()
{
	BuffIntersect that;

	that.nbIntersections = 0;
	that.origin = vcg::Point3d({ 0, 0, 0 });
	that.direction = vcg::Point3d({ 0, 0, 0 });
	that.time = 0;

	return that;
}

void debug_intesections()
{
	// Import du mesh
	mc::mvcg::Mesh mesh;
	if (!mc::mvcg::obj::loader(mesh, "../../obj/Sphere-632586.obj")) {
		logger::mcLog(stderr, "Impossible de lire l'obj");
		exit(-1);
	}

	// Création de l'octree
	mesh.TreeMake();

	std::vector<BuffIntersect> intersections;
	std::clock_t iStart;

	std::clock_t start = std::clock();
	for (int i = -6; i <= 6; ++i)
	{
		for (int j = -6; j <= 6; ++j)
		{
			iStart = std::clock();

			BuffIntersect it;

			it.origin = vcg::Point3d({ i * 10.0f, j * 10.0f, 100.0f });
			it.direction = vcg::Point3d({ 0.0f, 0.0f, -1.0f });

			it.nbIntersections = Intersect(mesh.face, mesh.tree, it.facesIntersections, it.pointsIntersections, it.origin, it.direction);

			it.time = std::clock() - iStart;

			intersections.push_back(it);
		}
	}
	std::clock_t end = std::clock();

	vcg::tri::io::ExporterOBJ<mc::mvcg::Mesh>::Save(mesh, "OUTOBJ.obj", 0);


	/************************ LOG DES RESULTATS *********************************/

	std::cout << "All intersections in " << end - start << "ms." << std::endl;

	int i = 0;
	for (auto elem : intersections)
	{
		logger::mcLog(logger::logStream, "Ligne\t%i ", i++);
		logger::mcLog(logger::logStream, "Origine(%f, %f, %f) - Direction(%f, %f, %f)\n",
			elem.origin[0], elem.origin[1], elem.origin[2], elem.direction[0], elem.direction[1], elem.direction[2]);
		logger::mcLog(logger::logStream, "\n");
	}

	logger::mcLog(logger::logStream, "\n==================================================================================================\n\n");
	logger::mcLog(logger::logStream, "\Durée des %i intersections = %ims.\n\n", intersections.size(), end - start);
	logger::mcLog(logger::logStream, "==================================================================================================\n\n\n");

	i = 0;
	for (auto elem : intersections)
	{
		logger::mcLog(logger::logStream, "\n\nLigne\t%i Origine(%f, %f, %f) - Direction(%f, %f, %f)\n", i++,
			elem.origin[0], elem.origin[1], elem.origin[2], elem.direction[0], elem.direction[1], elem.direction[2]);
		logger::mcLog(logger::logStream, "--------------------------------------------------------------------------------------------------\n");
		logger::mcLog(logger::logStream, "Nb. Int = %i -> %ims\n\n", elem.nbIntersections, elem.time);

		for (int iInt = 0; elem.nbIntersections > 0 && iInt < elem.nbIntersections; ++iInt)
		{
			logger::mcLog(logger::logStream, "[%i] = (%.7f, %.7f, %.7f)\n", iInt, elem.pointsIntersections[iInt].X(), elem.pointsIntersections[iInt].Y(), elem.pointsIntersections[iInt].Z());
		}
	}

	vcg::tri::io::ExporterOBJ<mc::mvcg::Mesh>::Save(mesh, "OUTOBJ.obj", 0);
}

void debug_intersection_v2(char *path)
{
	mc::mvcg::Mesh m;
	mc::mvcg::obj::loader(m, path);

	m.TreeMake();

	vcg::RayTriangleIntersectionFunctor<> rayIntersector;
	const MIndex::ScalarType maxDist = std::numeric_limits<double>::max();

	// Ray
	MIndex::CoordType rayOrigin(-0.5, 0.5, 100);
	MIndex::CoordType rayDirection(0.0f, 0.0f, -1.0f);

	rayDirection.normalized();
	vcg::Ray3<MIndex::ScalarType, true> ray(rayOrigin, rayDirection);

	MIndex::ObjPtr isectFace;
	MIndex::ScalarType intersectionDist;

	std::vector<BuffIntersect> intersections;
	std::clock_t iStart;

	std::clock_t start = std::clock();
	for (int i = -6; i <= 6; ++i)
	{
		for (int j = -6; j <= 6; ++j)
		{
			BuffIntersect it;
			ray.SetOrigin({ i * 10.0f, j * 10.0f, 100.0f });
			it.direction = rayDirection;
			it.origin = rayOrigin;
			iStart = std::clock();

			int nbInter = 0;
			while ((isectFace = m.tree.DoRay(rayIntersector, vcg::EmptyClass(), ray, maxDist, intersectionDist)) != 0)
			{
				nbInter++;
				it.facesIntersections.push_back(isectFace);
				it.pointsIntersections.push_back(getPositionWithDistAndDir(rayOrigin, rayDirection, intersectionDist));

				int cnt = 0;
				for (; &m.face[cnt] != isectFace; ++cnt);
				m.face.erase(m.face.begin() + cnt);
			}
			it.time = std::clock() - iStart;
			it.nbIntersections = nbInter;
			intersections.push_back(it);
		}
	}
	std::clock_t end = std::clock();

	/************************ LOG DES RESULTATS *********************************/

	std::cout << "All intersections in " << end - start << "ms." << std::endl;

	int i = 0;
	for (auto elem : intersections)
	{
		logger::mcLog(logger::logStream, "Ligne\t%i ", i++);
		logger::mcLog(logger::logStream, "Origine(%f, %f, %f) - Direction(%f, %f, %f)\n",
			elem.origin[0], elem.origin[1], elem.origin[2], elem.direction[0], elem.direction[1], elem.direction[2]);
		logger::mcLog(logger::logStream, "\n");
	}

	logger::mcLog(logger::logStream, "\n==================================================================================================\n\n");
	logger::mcLog(logger::logStream, "\Durée des %i intersections = %ims.\n\n", intersections.size(), end - start);
	logger::mcLog(logger::logStream, "==================================================================================================\n\n\n");

	i = 0;
	for (auto elem : intersections)
	{
		logger::mcLog(logger::logStream, "\n\nLigne\t%i Origine(%f, %f, %f) - Direction(%f, %f, %f)\n", i++,
			elem.origin[0], elem.origin[1], elem.origin[2], elem.direction[0], elem.direction[1], elem.direction[2]);
		logger::mcLog(logger::logStream, "--------------------------------------------------------------------------------------------------\n");
		logger::mcLog(logger::logStream, "Nb. Int = %i -> %ims\n\n", elem.nbIntersections, elem.time);

		for (int iInt = 0; elem.nbIntersections > 0 && iInt < elem.nbIntersections; ++iInt)
		{
			logger::mcLog(logger::logStream, "[%i] = (%.7f, %.7f, %.7f)\n", iInt, elem.pointsIntersections[iInt].X(), elem.pointsIntersections[iInt].Y(), elem.pointsIntersections[iInt].Z());
		}
	}

	vcg::tri::io::ExporterOBJ<mc::mvcg::Mesh>::Save(m, "OUTOBJ.obj", 0);
}