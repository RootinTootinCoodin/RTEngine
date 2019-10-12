#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"

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
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* m = scene->mMeshes[i];
			mesh* _mesh = new mesh;
			
			_mesh->num_vertices = m->mNumVertices;
			_mesh->vertices = new float[_mesh->num_vertices * 3];
			memcpy(_mesh->vertices, m->mVertices, sizeof(float) * _mesh->num_vertices * 3);
			LOG("New mesh with %d vertices", _mesh->num_vertices);

			if (m->HasTextureCoords(i))
			{
				_mesh->has_texture = true;
				_mesh->uvs = new float[_mesh->num_vertices*2];
				memcpy(_mesh->uvs, m->mTextureCoords, sizeof(float)*_mesh->num_vertices * 2);
			}

			if (m->HasFaces())
			{
				_mesh->num_indices = m->mNumFaces * 3;
				_mesh->indices = new uint[_mesh->num_indices];

				for (uint i = 0; i < m->mNumFaces; ++i)
				{
					if (m->mFaces[i].mNumIndices == 3)
						memcpy(&_mesh->indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
					else
						LOG("WARNING, geometry face without 3 indices !");
				}

				//TODO: UGLY
				App->renderer3D->GenerateBufferForMesh(_mesh);
				App->scene->model.push_back(_mesh);
				App->scene->model_loaded = true;
			}
			else
				LOG("Error mesh from scene %s, no faces", path);
		}
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);
	return true;
}

bool ModuleLoader::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}
