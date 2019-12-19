#ifndef MC_CGAL_OBJ_IMPORTER_CPP
#define MC_CGAL_OBJ_IMPORTER_CPP

#include "mc_cgal_obj_io.hpp"
#include <mc_utils.hpp>

#endif //!MC_CGAL_OBJ_IMPORTER_CPP

void mc::mcgal::SaveObj(mc::mcgal::Mesh& mesh, const char* path)
{
	std::clock_t start = std::clock();
	std::filebuf fb;
	fb.open(path, std::ios::out);
	std::ostream os(&fb);

	CGAL::print_polyhedron_wavefront<CGAL::Polyhedron_3<CGAL::Simple_cartesian<double>>>(os, mesh);

	fb.close();
	std::clock_t end = std::clock();

	mc::utils::traceWithTime(std::string(path) + std::string(" sauvegardé avec succés"), end - start);
}