#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")


#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"
#include "DevIL/include/ilu.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#pragma comment( lib, "Devil/libx86/ILUT.lib" )



ModuleLoader::ModuleLoader(Application * parent, bool start_enabled) : Module(parent)
{
	name = "Loader";
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

}

ModuleLoader::~ModuleLoader()
{
}

bool ModuleLoader::FileReceived(std::string path)
{
	if (App->scene->model_loaded == true)
	{
		LoadTexture(path);
	}
	else
		LoadFBX(path);
	return true;
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

			if (m->HasTextureCoords(0))
			{
				_mesh->has_texture = true;
				//_mesh->uvs = new float[_mesh->num_vertices*2];
				//
				//memcpy(_mesh->uvs, m->mTextureCoords, sizeof(float)*_mesh->num_vertices * 2);
				_mesh->uvs = new float[_mesh->num_vertices * 2];

				for (int t = 0; t < _mesh->num_vertices * 2; t += 2)
				{
					_mesh->uvs[t] = m->mTextureCoords[0][t / 2].x;
					_mesh->uvs[t + 1] = m->mTextureCoords[0][t / 2].y;
					LOG("UV: %f, %f",_mesh->uvs[t], _mesh->uvs[t + 1]);
				}
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

bool ModuleLoader::LoadTexture(std::string path)
{
	ILuint il_img_name = 0;
	ilGenImages(1,&il_img_name);
	ilBindImage(il_img_name);

	
	if (ilLoadImage(path.c_str()))
	{
		ILinfo il_img_data;
		iluGetImageInfo(&il_img_data);
		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			LOG("DevIL Error: %s", iluErrorString(ilGetError()));
		}

		if (il_img_data.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		//App->scene->GenerateTexture((uint*)ilGetData(), il_img_data.Width, il_img_data.Height);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &App->scene->id_image);
		glBindTexture(GL_TEXTURE_2D, App->scene->id_image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		ILubyte* data = ilGetData();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, il_img_data.Width, il_img_data.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
		LOG("Error loading texture: %s", iluErrorString(ilGetError()));
	ilDeleteImage(il_img_name);
	return false;
}

bool ModuleLoader::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}
