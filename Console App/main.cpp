#ifndef _CONSOLE_APP_MAIN_C
#define _CONSOLE_APP_MAIN_C

/**** STANDARD ****/
#include <iostream>
#include <ctime>
#include <Windows.h>

#define VCG_MY


/**** VCG ****/
#ifdef VCG_MY
#include <obj.h>
#endif //!VCG_MY


/**** OPEN MESH ****/
#ifdef OPEN_MESH_MY
#undef min
#undef max
#include <OpenMesh\Core\Mesh\PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh;
#endif //!OPEN_MESH_MY

#endif //!_CONSOLE_APP_MAIN_C

int main(void)
{

	std::clock_t start = 0, end = 0;
#ifdef OPEN_MESH_MY

	MyMesh mesh;

	try
	{
		start = std::clock();
		if (!OpenMesh::IO::read_mesh(mesh, "../../obj/M5.obj"))
		{
			OutputDebugString("Cannot read file 'output.obj'\n");
			return -1;
		}
	}
	catch (std::exception &e)
	{
		OutputDebugString(e.what());
		return -1;
	}
	end = std::clock();

	std::cout << "OBJ read in " << end - start << "ms." << std::endl;
	
#endif // OPEN_MESH_MY

#ifdef VCG_MY

	Mesh mesh;
	vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info;

	start = std::clock();
	vcg::tri::io::ImporterOBJ<Mesh>::Open(mesh, "../../obj/M5.obj", mesh_info);
	end = std::clock();

	std::cout << "OBJ read in " << end - start << "ms." << std::endl;

#endif //!VCG_MY


	/*MyMesh::VertexHandle vertexHandle[8];
	
	vertexHandle[0] = mesh.add_vertex(MyMesh::Point(-1, -1, 1));
	vertexHandle[1] = mesh.add_vertex(MyMesh::Point(1, -1, 1));
	vertexHandle[2] = mesh.add_vertex(MyMesh::Point(1, 1, 1));
	vertexHandle[3] = mesh.add_vertex(MyMesh::Point(-1, 1, 1));
	vertexHandle[4] = mesh.add_vertex(MyMesh::Point(-1, -1, -1));
	vertexHandle[5] = mesh.add_vertex(MyMesh::Point(1, -1, -1));
	vertexHandle[6] = mesh.add_vertex(MyMesh::Point(1, 1, -1));
	vertexHandle[7] = mesh.add_vertex(MyMesh::Point(-1, 1, -1));

	std::vector<MyMesh::VertexHandle> faceVertexHandles;

	faceVertexHandles.clear();
	faceVertexHandles.push_back(vertexHandle[0]);
	faceVertexHandles.push_back(vertexHandle[1]);
	faceVertexHandles.push_back(vertexHandle[2]);
	faceVertexHandles.push_back(vertexHandle[3]);
	mesh.add_face(faceVertexHandles);

	faceVertexHandles.clear();
	faceVertexHandles.push_back(vertexHandle[7]);
	faceVertexHandles.push_back(vertexHandle[6]);
	faceVertexHandles.push_back(vertexHandle[5]);
	faceVertexHandles.push_back(vertexHandle[4]);
	mesh.add_face(faceVertexHandles);

	faceVertexHandles.clear();
	faceVertexHandles.push_back(vertexHandle[1]);
	faceVertexHandles.push_back(vertexHandle[0]);
	faceVertexHandles.push_back(vertexHandle[4]);
	faceVertexHandles.push_back(vertexHandle[5]);
	mesh.add_face(faceVertexHandles);

	faceVertexHandles.clear();
	faceVertexHandles.push_back(vertexHandle[2]);
	faceVertexHandles.push_back(vertexHandle[1]);
	faceVertexHandles.push_back(vertexHandle[5]);
	faceVertexHandles.push_back(vertexHandle[6]);
	mesh.add_face(faceVertexHandles);

	faceVertexHandles.clear();
	faceVertexHandles.push_back(vertexHandle[3]);
	faceVertexHandles.push_back(vertexHandle[2]);
	faceVertexHandles.push_back(vertexHandle[6]);
	faceVertexHandles.push_back(vertexHandle[7]);
	mesh.add_face(faceVertexHandles);

	faceVertexHandles.clear();
	faceVertexHandles.push_back(vertexHandle[0]);
	faceVertexHandles.push_back(vertexHandle[3]);
	faceVertexHandles.push_back(vertexHandle[7]);
	faceVertexHandles.push_back(vertexHandle[4]);
	mesh.add_face(faceVertexHandles);
	
	try
	{
		if (!OpenMesh::IO::write_mesh(mesh, "output.obj"))
		{
			std::cerr << "Cannot write mesh to file 'output.obj'" << std::endl;
			return 1;
		}
	}
	catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		return 1;
	}*/

	std::getchar();
	std::getchar();

	return 0;

}