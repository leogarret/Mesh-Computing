#include "mc_cgal_mesh.hpp"
#include <ctime>

CMesh::CMesh()
{
}

CMesh::CMesh(char* path)
{
	std::clock_t start = std::clock();
	std::ifstream input(path);

	if (!input.is_open())
	{
		std::cerr << "File not open (" << path << ")." << std::endl;;
		return;
	}

	if (!CGAL::read_OBJ(input, mvertices, mfaces))
	{
		std::cerr << "OBJ can't be read." << std::endl;
		return;
	}

	_is_loading = true; // Le maillage est bien chargé
	std::clock_t end = std::clock();
	printf("Load in %ims.\n", end - start);
}

void CMesh::save(char* path)
{
	std::filebuf fb;
	fb.open(path, std::ios::out);
	std::ostream output(&fb);

	CGAL::write_STL<CGAL::Surface_mesh<K::Point_3>>(*this, output);
}

