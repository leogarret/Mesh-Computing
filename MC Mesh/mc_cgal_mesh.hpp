#ifndef MC_CGAL_MESH_HPP
#define MC_CGAL_MESH_HPP

/* CGAL */
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/AABB_face_graph_triangle_primitive.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/OBJ_reader.h>
#include <CGAL/IO/STL_writer.h>
#include <CGAL/Point_3.h>
#include <CGAL/IO/print_wavefront.h>

#include <CGAL/intersection_3.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>
#include <CGAL/Simple_cartesian.h>

/* STD */
#include <iostream>
#include <fstream>
#include <string>
#include <exception>

typedef CGAL::Exact_predicates_inexact_constructions_kernel						Kernel;
typedef CGAL::Simple_cartesian<double>											K;
typedef K::Point_3																Point;
typedef K::Plane_3																Plane;
typedef K::Segment_3															Segment;
typedef K::Ray_3																Ray;
typedef CGAL::Polyhedron_3<K>													Polyhedron;
typedef CGAL::AABB_face_graph_triangle_primitive<Polyhedron>					Primitive;
typedef CGAL::AABB_traits<K, Primitive>											Traits;
typedef CGAL::AABB_tree<Traits>													Tree;
typedef boost::optional<Tree::Intersection_and_primitive_id<Segment>::Type>		Segment_intersection;

namespace mc::mcgal
{

	class Mesh : public CGAL::Surface_mesh<K::Point_3>, public CGAL::Polyhedron_3<K>
	{
	public:
		inline bool isLoading() { return _is_loading; };
		inline bool treeIsMake() { return _tree_make; };

		std::vector<CGAL::Point_3<Kernel>> mvertices;
		std::vector<std::vector<size_t>> mfaces;

		Tree tree;
		
	private:
		bool _is_loading = false;
		bool _tree_make = false;
	};

	/*
	** Cette class permet d'avoir la possibilité de manipuler les maillages au format obj
	*/
	template<class HDS>
	class BuildCgalPolyhedronFromObj : public CGAL::Modifier_base<HDS>
	{
	public:
		BuildCgalPolyhedronFromObj(const std::string& fileName) : mFileName(fileName) {}

		void operator() (HDS& hds)
		{
			typedef typename HDS::Vertex   Vertex;
			typedef typename Vertex::Point Point;

			// Ouverture du fichier obj
			std::ifstream _file(mFileName.c_str());
			if (!_file)
			{
				return;
			}

			// On compte le nombre de sommets
			std::string _line;
			int _numVertices = 0;
			int _numFacets = 0;
			while (_file.good())
			{
				std::getline(_file, _line);
				if (_line.size() > 1)
				{
					if (_line[0] == 'v' && _line[1] == ' ') { ++_numVertices; }
					if (_line[0] == 'f' && _line[1] == ' ') { ++_numFacets; }
				}
			}

			if (!_file.good())
			{
				_file.clear();
			}
			_file.seekg(0);

			// On verifie la validité du maillage avant de le créer
			CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);

			// chargement des données
			B.begin_surface(_numVertices, _numFacets, int((_numVertices + _numFacets - 2)*2.1));

			std::string _token;
			while (!_file.eof())
			{
				_token = "";
				_file >> _token;

				// si _token stocke "v", c'est un sommet
				if (_token == "v")
				{
					double x, y, z;
					_file >> x >> y >> z;
					B.add_vertex(Point(x, y, z));
				}
				else if (_token == "f") // Si _token stocke "f", c'est une face
				{
					std::string _line;
					std::getline(_file, _line);

					std::istringstream _stream(_line);
					std::vector<std::string> _vertices;
					std::copy(std::istream_iterator<std::string>(_stream),
						std::istream_iterator<std::string>(),
						std::back_inserter(_vertices));

					B.begin_facet();
					for (size_t i = 0; i<_vertices.size(); ++i)
					{
						std::string::size_type _pos = _vertices[i].find('/', 0);
						std::string _indexStr = _vertices[i].substr(0, _pos);
						B.add_vertex_to_facet(stoi(_indexStr) - 1);
					}
					B.end_facet();
				}
			}
			_file.close();
			B.end_surface();
		}

	private:
		std::string mFileName;
	};

	/*
	** Cette fonction permet d'importer un maillage au format OBJ avec CGAL
	*/
	template<class _Mesh>
	void cgOpenObj(_Mesh& mesh, const char* path)
	{
		BuildCgalPolyhedronFromObj<_Mesh::HalfedgeDS> _buildPolyhedron(path);

		mesh.delegate(_buildPolyhedron);
	}

};

#endif //!MC_CGAL_MESH_HPP