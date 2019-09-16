#ifndef _ASSIMP_OBJ_LOADER_C
#define _ASSIMP_OBJ_LOADER_C

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#endif // !_ASSIMP_OBJ_LOADER_C

bool DoTheImportThing(const std::string& pFile)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene) {
		std::cerr << importer.GetErrorString() << std::endl;
		return false;
	}
	// Now we can access the file's contents.
	DoTheSceneProcessing(scene);

	return true;
}