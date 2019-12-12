#ifndef _MC_CGAL_MESH_HPP
#define _MC_CGAL_MESH_HPP

/* CGAL */
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/AABB_face_graph_triangle_primitive.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/OBJ_reader.h>
#include <CGAL/IO/STL_writer.h>
#include <CGAL/Point_3.h>
#include <CGAL/IO/print_wavefront.h>

/* STD */
#include <iostream>
#include <fstream>

/* MY */

typedef CGAL::Exact_predicates_inexact_constructions_kernel	Kernel;
typedef CGAL::Simple_cartesian<double> K;

class CMesh : public CGAL::Surface_mesh<K::Point_3>
{
public:
	CMesh();
	CMesh(char *path);

	inline bool isLoading() { return _is_loading; };

	void save(char* path);

	std::vector<CGAL::Point_3<Kernel>> mvertices;
	std::vector<std::vector<size_t>> mfaces;

private:
	bool _is_loading = false;
};

#endif // !_MC_CGAL_MESH_HPP