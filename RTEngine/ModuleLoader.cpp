#include "ModuleLoader.h"
#include "ModuleScene.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleLoader::ModuleLoader(Application * parent, bool start_enabled) : Module(parent)
{
	name = "Loader";
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

ModuleLoader::~ModuleLoader()
{
}

bool ModuleLoader::LoadFBX(std::string path)
{
	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		aiMesh* m = scene->mMeshes[0];
		mesh* mesh;

		mesh->num_vertices = m->mNumVertices;
		mesh->vertices = new float[mesh->num_vertices * 3];
		memcpy(mesh->vertices, m->mVertices, sizeof(float) * mesh->num_vertices * 3);
		LOG("New mesh with %d vertices", mesh->num_vertices);
		
		if (m->HasFaces())
		{
			mesh->num_indices = m->mNumFaces * 3;
			mesh->indices = new uint[mesh->num_indices];
			
			for (uint i = 0; i < m->mNumFaces; ++i)
			{
				if (m->mFaces[i].mNumIndices == 3)
					memcpy(&mesh->indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
				else
					LOG("WARNING, geometry face without 3 indices !");
			}
		}
		else
			LOG("Error mesh from scene %s, no faces", path);

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);
	return true;
}

bool ModuleLoader::GenerateBufferForMesh(mesh * mesh)
{
	

	return true;
}

bool ModuleLoader::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}
