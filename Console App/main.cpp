#ifndef _MAIN_C
#define _MAIN_C

#include <mc_tests.h>
#include <mc_vcg_algorithms.hpp>
#include <wrap\io_trimesh\export_obj.h>

#include <logger.hpp>
using namespace logger;

#include <vcg/complex/algorithms/hole.h>

#include <ctime>
#include <assert.h>
#include <limits.h>

#endif // !_MAIN_C

struct Edge
{
	float x1 = 0, y1 = 0, z1 = 0;
	float x2 = 0, y2 = 0, z2 = 0;
	/*float p1[3] = { 0, 0, 0 };
	float p2[3] = { 0, 0, 0 };*/
};

bool alreadyExists(const float edge_to_find[2][3], std::vector<Edge> edges)
{
	for (auto elem : edges)
	{
		if ((edge_to_find[0][0] == elem.x1 &&
			edge_to_find[0][1] == elem.y1 &&
			edge_to_find[0][2] == elem.z1 &&

			edge_to_find[1][0] == elem.x2 &&
			edge_to_find[1][1] == elem.y2 &&
			edge_to_find[1][2] == elem.z2)
			||
			(edge_to_find[0][0] == elem.x2 &&
			edge_to_find[0][1] == elem.y2 &&
			edge_to_find[0][2] == elem.z2 &&

			edge_to_find[1][0] == elem.x1 &&
			edge_to_find[1][1] == elem.y1 &&
			edge_to_find[1][2] == elem.z1))
		{
			return true;
		}
	}
	return false;
}

int main(int ac, char **av)
{
	mc::mvcg::Mesh m1;
	mc::mvcg::obj::loader(m1, "../../obj/BARLAM.obj");

	/*MyFace *f = m1.vert.at(0).VFp();

	if (m1.vert[0].HasVFAdjacency())
	{
		std::cout << "Adj\n" << std::endl;
	}

	std::cout << f[0].P(0)[0] << std::endl;*/
	

	std::clock_t start = std::clock();
	vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::FaceIterator fit = m1.face.begin();
	vcg::tri::io::ImporterOBJ<mc::mvcg::Mesh>::FaceIterator fit_end = m1.face.end();
	std::vector<Edge> edges;
	Edge edge_buff;
	float p_buff[2][3];
	for (int i = 0; fit != fit_end; ++fit)
	{
		std::cout << ++i << std::endl;
		p_buff[0][0] = fit[0].P(0)[0]; p_buff[0][1] = fit[0].P(0)[1]; p_buff[0][2] = fit[0].P(0)[2];
		p_buff[1][0] = fit[0].P(1)[0]; p_buff[1][1] = fit[0].P(1)[1]; p_buff[1][2] = fit[0].P(1)[2];
		if (alreadyExists(p_buff, edges) == false)
		{
			edge_buff.x1 = p_buff[0][0]; edge_buff.y1 = p_buff[0][1]; edge_buff.z1 = p_buff[0][2];
			edge_buff.x2 = p_buff[1][0]; edge_buff.y2 = p_buff[1][1]; edge_buff.z2 = p_buff[1][2];
			edges.push_back(edge_buff);
		}

		p_buff[0][0] = fit[0].P(0)[0]; p_buff[0][1] = fit[0].P(0)[1]; p_buff[0][2] = fit[0].P(0)[2];
		p_buff[0][0] = fit[0].P(2)[0]; p_buff[0][1] = fit[0].P(2)[1]; p_buff[0][2] = fit[0].P(2)[2];
		if (alreadyExists(p_buff, edges) == false)
		{
			edge_buff.x1 = p_buff[0][0]; edge_buff.y1 = p_buff[0][1]; edge_buff.z1 = p_buff[0][2];
			edge_buff.x2 = p_buff[1][0]; edge_buff.y2 = p_buff[1][1]; edge_buff.z2 = p_buff[1][2];
			edges.push_back(edge_buff);
		}

		p_buff[0][0] = fit[0].P(1)[0]; p_buff[0][1] = fit[0].P(1)[1]; p_buff[0][2] = fit[0].P(1)[2];
		p_buff[1][0] = fit[0].P(2)[0]; p_buff[1][1] = fit[0].P(2)[1]; p_buff[1][2] = fit[0].P(2)[2];
		if (alreadyExists(p_buff, edges) == false)
		{
			edge_buff.x1 = p_buff[0][0]; edge_buff.y1 = p_buff[0][1]; edge_buff.z1 = p_buff[0][2];
			edge_buff.x2 = p_buff[1][0]; edge_buff.y2 = p_buff[1][1]; edge_buff.z2 = p_buff[1][2];
			edges.push_back(edge_buff);
		}
	}
	std::clock_t end = std::clock();

	std::cout << end - start << std::endl;
	/*int i = 0;
	for (auto elem : edges)
	{
		std::cout << ++i << " = [" << elem.x1 << "," << elem.y1 << "," << elem.z1 << "] ";
		std::cout << "[" << elem.x2 << "," << elem.y2 << "," << elem.z2 << "]\n" << std::endl;
	}*/

	std::getchar();

	return 0;
}