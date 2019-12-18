#ifndef MC_CGAL_OBJ_IMPORTER_CPP
#define MC_CGAL_OBJ_IMPORTER_CPP

#include "mc_cgal_obj_io.hpp"

#endif //!MC_CGAL_OBJ_IMPORTER_CPP

void mc::mcgal::cgSaveObj(mc::mcgal::Mesh& mesh, const char* path)
{
	std::filebuf fb;
	fb.open(path, std::ios::out);
	std::ostream os(&fb);

	CGAL::print_polyhedron_wavefront<CGAL::Polyhedron_3<CGAL::Simple_cartesian<double>>>(os, mesh);

	fb.close();
}