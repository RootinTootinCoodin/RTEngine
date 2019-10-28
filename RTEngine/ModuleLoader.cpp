#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleFileSystem.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")


#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"
#include "DevIL/include/ilu.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#pragma comment( lib, "Devil/libx86/ILUT.lib" )

void AssimpLOG(const char*  message , char*  user )
{
	OutputDebugString(message);
	_app->app_log(message);
}

ModuleLoader::ModuleLoader(Application * parent, bool start_enabled) : Module(parent)
{
	name = "Loader";

	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, App->logs.c_str());
	stream.callback = AssimpLOG;
	aiAttachLogStream(&stream);

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

}

ModuleLoader::~ModuleLoader()
{
	aiDetachAllLogStreams();
}

bool ModuleLoader::FileReceived(std::string path)
{
	std::string extension;
	std::string name;
	App->fileSystem->SplitFilePath(path.c_str(),nullptr, &name, &extension);
	if (MODEL_EXTENSIONS(extension))
		LoadFBX(path, name);
	if (TEXTURE_EXTENSIONS(extension))
		LoadTexture(path);
	return true;
}

bool ModuleLoader::LoadFBX(std::string& path, std::string& name)
{
	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		GameObject* new_model = App->scene->root->AddChildren(name);
		
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			LoadMesh(scene->mMeshes[i], new_model,scene,path);
		}
		aiReleaseImport(scene);
		App->camera->AdjustCameraToAABB(new_model->GetAABB());
	}
	else
		LOG("Error loading scene %s", path);
	return true;
}

bool ModuleLoader::LoadTexture(std::string& path, ComponentMaterial* material)
{
	bool ret = false;
	ILuint il_img_name = 0;
	ilGenImages(1,&il_img_name);
	ilBindImage(il_img_name);

	if (ilLoadImage(path.c_str()))
	{
		texture* new_texture = new texture;
		ILinfo il_img_data;
		iluGetImageInfo(&il_img_data);

		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
			LOG("Error converting image: %s", iluErrorString(ilGetError()));
		
		//Fuck different coordinate systems
		if (il_img_data.Origin == IL_ORIGIN_UPPER_LEFT)
			if (!iluFlipImage())
				LOG("Error rotating image: %s", iluErrorString(ilGetError()));

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &new_texture->id_texture);
		glBindTexture(GL_TEXTURE_2D, new_texture->id_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		ILubyte* data = ilGetData();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, il_img_data.Width, il_img_data.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
		glBindTexture(GL_TEXTURE_2D, 0);

		new_texture->path = path;
		new_texture->width = il_img_data.Width;
		new_texture->height = il_img_data.Height;
		new_texture->depth = il_img_data.Depth;
		new_texture->bpp = il_img_data.Bpp;
		App->fileSystem->SplitFilePath(path.c_str(), nullptr, &new_texture->name, nullptr);


		App->scene->textures.push_back(new_texture);

		if (material)
			material->CopyTextureToThis(new_texture);
		else
			App->scene->root->RecursiveApplyTexture(new_texture);

		ret = true;
	}
	else
		LOG("Error loading texture: %s", path.c_str());

	ilDeleteImage(il_img_name);
	return ret;
}

void ModuleLoader::LoadVertices(ComponentMesh * _mesh, aiMesh * m)
{
	_mesh->num_vertices = m->mNumVertices;
	_mesh->vertices = new float[_mesh->num_vertices * 3];
	memcpy(_mesh->vertices, m->mVertices, sizeof(float) * _mesh->num_vertices * 3);
	LOG("New mesh with %d vertices", _mesh->num_vertices);
}

void ModuleLoader::LoadUVS(ComponentMesh * _mesh, aiMesh * m)
{
	//Can't use memcpy because m->mTextureCoords is a 3D vector but we only use x and y
	_mesh->uvs = new float[_mesh->num_vertices * 2];
	for (int t = 0; t < _mesh->num_vertices * 2; t += 2)
	{
		_mesh->uvs[t] = m->mTextureCoords[0][t / 2].x;
		_mesh->uvs[t + 1] = m->mTextureCoords[0][t / 2].y;
	}
}

void ModuleLoader::LoadNormals(ComponentMesh * _mesh, aiMesh * m)
{
	_mesh->normals = new float[_mesh->num_vertices * 3];
	memcpy(_mesh->normals, m->mNormals, sizeof(float) * _mesh->num_vertices * 3);
	LOG("Normals copied");
}

void ModuleLoader::LoadMeshTexture(ComponentMaterial * _material, aiMaterial* material, std::string& path)
{
	aiString texture_name;
	material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_name);

	std::string texture_path;
	App->fileSystem->SplitFilePath(path.c_str(), &texture_path);
	texture_path += texture_name.C_Str();
	if (!LoadTexture(texture_path, _material))
	{
		std::string texture_path_2 = App->fileSystem->GetWritePath();
		texture_path_2 += ASSETS_TEXTURES_FOLDER;
		texture_path_2 += texture_name.C_Str();
		LOG("Texture is not located in the same path as the mesh");
		if (LoadTexture(texture_path_2, _material))
			LOG("Texture found in the Textures folder");
	}
}

void ModuleLoader::LoadMeshFaces(ComponentMesh * _mesh, aiMesh * m)
{
	_mesh->num_indices = m->mNumFaces * 3;
	_mesh->indices = new uint[_mesh->num_indices];

	for (uint k = 0; k < m->mNumFaces; ++k)
	{
		if (m->mFaces[k].mNumIndices == 3)
			memcpy(&_mesh->indices[k * 3], m->mFaces[k].mIndices, 3 * sizeof(uint));
		else
			LOG("WARNING, geometry face without 3 indices !");
	}
}

void ModuleLoader::LoadMesh(aiMesh * m, GameObject* new_model, const aiScene* scene, std::string& path)
{
	GameObject* mesh_gameobject = new_model->AddChildren(m->mName.C_Str());

	ComponentMesh* _mesh = (ComponentMesh*)mesh_gameobject->AddComponent(MESH);
	ComponentMaterial* _material = (ComponentMaterial*)mesh_gameobject->AddComponent(MATERIAL);

	LoadVertices(_mesh, m);

	if (m->HasTextureCoords(0))
	{
		LoadUVS(_mesh, m);
	}

	if (m->HasNormals())
	{
		LoadNormals(_mesh, m);
	}

	if (aiMaterial* material = scene->mMaterials[m->mMaterialIndex])
	{
		LoadMeshTexture(_material, material, path);
	}
	else
		_material->CopyTextureToThis(App->scene->textures[0]);

	if (m->HasVertexColors(0))
	{
		_mesh->colors = new float[_mesh->num_vertices * 4];
		memcpy(_mesh->colors, m->mColors[0], sizeof(float)*_mesh->num_vertices * 4);
	}

	if (m->HasFaces())
	{
		LoadMeshFaces(_mesh,m);
		App->renderer3D->GenerateBufferForMesh(_mesh);
		mesh_gameobject->ParentRecalculateAABB();
	}
	else
		LOG("Error mesh from scene %s, no faces", path);
}

bool ModuleLoader::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}
