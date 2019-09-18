#pragma once
#include "main.h"
#include "openMesh.h"

// VCG_MY if using VCG and OPEN_MESH_MY if using OpenMesh
//#define VCG_MY

int main(void)
{
	om::Mesh mesh;

	if (!om::open_mesh(mesh, "../../obj/M5.obj"))
		return (-1);

	return 0;
}