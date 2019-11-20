#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "FileSystem.h"


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
	FileSystem::SplitFilePath(path,nullptr, &name, &extension);
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
		
	/*	for (int i = 0; i < scene->mNumMeshes; i++)
		{

			LoadMesh(scene->mMeshes[i], new_model,scene,path);
		}*/
		ComponentTransform* transform = (ComponentTransform*)new_model->GetComponent(TRANSFORM);
		LoadAiNodesRecursively(scene->mRootNode, scene, new_model, path,transform->GetLocalTransformMatrix());
		aiReleaseImport(scene);
		App->camera->AdjustCameraToAABB(new_model->GetAABB());
	}
	else
		LOG("Error loading scene %s", path);
	return true;
}

bool ModuleLoader::LoadAiNodesRecursively(aiNode * node, const aiScene* scene,GameObject* parent, std::string& path, math::float4x4 cumulative_transform)
{
	GameObject* go = nullptr;

	aiVector3D translation, scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	math::float4x4 node_transform = float4x4::FromTRS(pos,rot.ToFloat4x4(),scale);
	math::float4x4 transform = cumulative_transform * node_transform;

	if (node->mNumMeshes > 0)
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{

			go = LoadMesh(scene->mMeshes[node->mMeshes[i]], parent, scene, path);
			ComponentTransform* transform_comp = (ComponentTransform*)go->GetComponent(TRANSFORM);
			transform_comp->setLocalFromMatrix(transform);
		}
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		if(go)
			LoadAiNodesRecursively(node->mChildren[i], scene, go, path,transform);
		else
			LoadAiNodesRecursively(node->mChildren[i], scene, parent, path, transform);

	}
	return true;
}

bool ModuleLoader::LoadTexture(std::string& path, ComponentMaterial* material)
{
	bool ret = false;
	ILuint il_img_name = 0;
	ilGenImages(1,&il_img_name);
	ilBindImage(il_img_name);

	std::string path2 = ".";
	path2 += path;
	if (ilLoadImage(path2.c_str()))
	{

		ILinfo il_img_info;
		iluGetImageInfo(&il_img_info);

		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
			LOG("Error converting image: %s", iluErrorString(ilGetError()));
		
		//Fuck different coordinate systems
		if (il_img_info.Origin == IL_ORIGIN_UPPER_LEFT)
			if (!iluFlipImage())
				LOG("Error rotating image: %s", iluErrorString(ilGetError()));

		std::string name;
		std::string extension;

		FileSystem::SplitFilePath(path, nullptr, &name, &extension);

		LoadMaterial(il_img_info, path, name, material);

		SaveTextureAsDDS(name);
		
		ret = true;
	}
	else
		LOG("Error loading texture: %s", path.c_str());

	ilDeleteImage(il_img_name);
	return ret;
}

void ModuleLoader::LoadTransform(aiNode * node, GameObject * game_object)
{

	aiVector3D translation, scaling; 
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z); 
	float3 scale(scaling.x, scaling.y, scaling.z); 
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	ComponentTransform* transform = (ComponentTransform*)game_object->GetComponent(TRANSFORM);
	transform->setPos(pos);
	transform->setScale(scale);
	transform->setRotation(rot);
	transform->setLocalFromPSR();

}


bool ModuleLoader::LoadMaterial(ILinfo& il_img_info,std::string& path, std::string& name, ComponentMaterial* material)
{
	texture* new_texture = new texture;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &new_texture->id_texture);
	glBindTexture(GL_TEXTURE_2D, new_texture->id_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, il_img_info.Width, il_img_info.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
	glBindTexture(GL_TEXTURE_2D, 0);

	new_texture->path = path;
	new_texture->width = il_img_info.Width;
	new_texture->height = il_img_info.Height;
	new_texture->depth = il_img_info.Depth;
	new_texture->bpp = il_img_info.Bpp;
	new_texture->name = name;
	App->scene->textures.push_back(new_texture);

	if (material)
		material->CopyTextureToThis(new_texture);
	else
		App->scene->root->RecursiveApplyTexture(new_texture);

	return true;
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
	FileSystem::SplitFilePath(path, &texture_path);
	texture_path += texture_name.C_Str();
	if (!LoadTexture(texture_path, _material))
	{
		std::string texture_path_2 =  ASSETS_TEXTURES_FOLDER;
		texture_path_2 += texture_name.C_Str();
		LOG("Texture is not located in the same path as the mesh");
		if (LoadTexture(texture_path_2, _material))
			LOG("Texture found in the Textures folder");
	}
}

bool ModuleLoader::LoadMeshFaces(ComponentMesh * _mesh, aiMesh * m)
{
	_mesh->num_indices = m->mNumFaces * 3;
	_mesh->indices = new uint[_mesh->num_indices];

	for (uint k = 0; k < m->mNumFaces; ++k)
	{
		if (m->mFaces[k].mNumIndices == 3)
			memcpy(&_mesh->indices[k * 3], m->mFaces[k].mIndices, 3 * sizeof(uint));
		else
		{
			LOG("WARNING, geometry face without 3 indices !");
			return false;
		}
	}
	return true;
}

GameObject* ModuleLoader::LoadMesh(aiMesh * m, GameObject* new_model, const aiScene* scene, std::string& path)
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
		if (LoadMeshFaces(_mesh, m))
		{
			App->renderer3D->GenerateBufferForMesh(_mesh);
			mesh_gameobject->ParentRecalculateAABB();

		}
		else
		{
			mesh_gameobject->RemoveComponent(MESH);
			mesh_gameobject->RemoveComponent(MATERIAL);
		}

	}
	else
		LOG("Error mesh from scene %s, no faces", path);

	return mesh_gameobject;
}

bool ModuleLoader::SaveTextureAsDDS(std::string& name)
{
	bool ret = false;
	ILuint size;
	ILubyte* data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
			ret = FileSystem::ExportBuffer((char*)data, size, name.c_str(), LIBRARY_TEXTURES, ".dds");
		RELEASE_ARRAY(data);
	}
	return ret;
}

bool ModuleLoader::ImportMesh(aiMesh* mesh)
{
	if (mesh->HasFaces())
	{
		//GOD BLESS C++ 
		uint uvs = mesh->HasTextureCoords(0);
		uint normals = mesh->HasNormals();
		uint colors = mesh->HasVertexColors(0);

		//Header 
		uint ranges[5] = { mesh->mNumVertices,mesh->mNumFaces * 3 ,uvs,normals,colors};
		//Calculate the size of the file
		uint size = sizeof(ranges) + sizeof(uint) * mesh->mNumFaces * 3 + sizeof(float) * mesh->mNumVertices * 3;
		if (uvs)
			size += sizeof(float) * mesh->mNumVertices * 2;
		
		if (normals)
			size += sizeof(float) * mesh->mNumVertices * 3;

		if (colors)
			size += sizeof(float) * mesh->mNumVertices * 4;

		//Data: File, Cursor: Pointer to where we are writing in the file
		char* data = new char[size];
		char* cursor = data;

		//Store the ranges first
		uint bytes = sizeof(ranges);
		memcpy(cursor, ranges, bytes);

		//Store Vertices
		cursor += bytes; //We "move" the cursor a distance equal to the memory copied previously;
		bytes = sizeof(float) * mesh->mNumVertices * 3;
		memcpy(cursor, mesh->mVertices, bytes);

		//Store Indices

		cursor += bytes;
		bytes = sizeof(uint) * mesh->mNumFaces * 3;
		for (uint k = 0; k < mesh->mNumFaces; ++k)
		{
			if (mesh->mFaces[k].mNumIndices == 3)
				memcpy(&cursor[k * 3], mesh->mFaces[k].mIndices, 3 * sizeof(uint));
			else
				LOG("WARNING, geometry face without 3 indices !");
		}

		cursor += bytes;
		if (uvs)
		{
			bytes = sizeof(float) * mesh->mNumVertices * 2;
			for (int t = 0; t < mesh->mNumVertices * 2; t += 2)
			{
				memcpy(&cursor[t], &mesh->mTextureCoords[0][t / 2].x, sizeof(float));
				memcpy(&cursor[t+1], &mesh->mTextureCoords[0][t / 2].y, sizeof(float));
			}
			cursor += bytes;
		}

		if (normals)
		{
			bytes = sizeof(float) * mesh->mNumVertices * 3;
			memcpy(cursor, mesh->mNormals, bytes);
			cursor += bytes;
		}

		if (colors)
		{
			bytes = sizeof(float) * mesh->mNumVertices * 4;
			memcpy(cursor, mesh->mColors[0], bytes);
		}

		
	}
	else
		LOG("Trying to Import a mesh without faces: %s", mesh->mName.C_Str());

	
	return false;
}

bool ModuleLoader::ImportGameobject(GameObject * go,char* pre_buffer)
{
	//size order: name, uuid, parent uuid, component uuids, active, static, 
	uint num_components = go->GetNumComponents();
	uint size = sizeof(go->GetName()) + sizeof(uint) + sizeof(uint) + sizeof(uint)*num_components + sizeof(bool) * 2;
	char* data = new char[size];
	char* cursor = data;


	uint bytes = sizeof(go->GetName());
	memcpy(cursor, &go->GetName(), bytes);

	//uuid
	cursor += bytes;
	bytes = sizeof(uint);
	uint uuid = go->GetUUID();
	memcpy(cursor,&uuid, bytes);

	//parent uuid
	cursor += bytes;
	bytes = sizeof(uint);
	uuid = go->GetParentUUID();
	memcpy(cursor, &uuid, bytes);



	//component UUIDS
	std::map<uint, Component*> components = go->GetComponentList();

	uint component_size = components.size();
	cursor += bytes;
	bytes = sizeof(uint);
	memcpy(cursor, &component_size, bytes);

	for (auto item = components.begin(); item != components.end(); item++)
	{
		cursor += bytes;
		bytes = sizeof(uint);
		uint uuid = (*item).first;
		memcpy(cursor, &uuid, bytes);
	}

	//active
	cursor += bytes;
	bytes = sizeof(bool);
	memcpy(cursor, &go->active, bytes);

	cursor += bytes;
	bytes = sizeof(bool);
	memcpy(cursor, &go->is_static, bytes);


	return false;
}

bool ModuleLoader::ImportComponent(Component * component, char * buffer)
{
	return false;
}

bool ModuleLoader::ExportMesh(ComponentMesh* mesh, char* buffer)
{
	char* cursor = buffer;
	uint ranges[5];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	mesh->num_vertices = ranges[0];
	mesh->num_indices = ranges[1];
	cursor += bytes;

	bytes = sizeof(float) * mesh->num_vertices * 3;
	mesh->vertices = new float[mesh->num_vertices * 3];
	memcpy(mesh->vertices, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(uint) * mesh->num_indices;
	mesh->indices = new uint[mesh->num_indices];
	memcpy(mesh->indices, cursor, bytes);
	cursor += bytes;


	if (ranges[2])
	{
		bytes = sizeof(float) * mesh->num_vertices * 2;
		mesh->uvs = new float[mesh->num_vertices * 2];
		memcpy(mesh->uvs, cursor, bytes);
		cursor += bytes;
	}
	if (ranges[3])
	{
		bytes = sizeof(float) * mesh->num_vertices * 3;
		mesh->vertices = new float[mesh->num_vertices * 3];
		memcpy(mesh->vertices, cursor, bytes);
		cursor += bytes;
	}
	if (ranges[4])
	{
		bytes = sizeof(float) * mesh->num_vertices * 4;
		mesh->colors = new float[mesh->num_vertices * 4];
		memcpy(mesh->colors, cursor, bytes);
		cursor += bytes;
	}


	return false;
}

uint ModuleLoader::ExportGameObject(char * buffer, std::vector<GameObject*> gameObjects_buffer)
{
	GameObject* new_go = new GameObject();

	char* cursor = buffer;

	uint size_of_name = 0;
	uint bytes = sizeof(uint);
	memcpy(&size_of_name, cursor, bytes);
	cursor += bytes;

	bytes = size_of_name;
	std::string name(cursor, size_of_name);
	new_go->SetName(name);
	cursor += bytes;

	bytes = sizeof(uint);
	uint uuid = 0;
	memcpy(&uuid, cursor, bytes);
	new_go->SetUUID(uuid);
	cursor += bytes;

	bytes = sizeof(uint);
	uint puuid = 0;
	memcpy(&puuid, cursor, bytes);
	new_go->SetParentUUID(puuid);
	cursor += bytes;

	bytes = sizeof(uint);
	uint num_of_components = 0;
	memcpy(&num_of_components, cursor, bytes);
	cursor += bytes;

	uint comp_uuid = 0;
	for (int i = 0; i < num_of_components; i++)
	{
		bytes = sizeof(uint);
		memcpy(&comp_uuid, cursor, bytes);
		cursor += bytes;
		ExportComponent(comp_uuid, new_go,cursor);
	}


	return false;
}

uint ModuleLoader::ExportComponent(uint uuid, GameObject * go, char* buffer)
{
	return uint();
}

bool ModuleLoader::ExportScene(char * buffer)
{
	char* cursor = buffer;
	uint num_game_objects;
	uint bytes = sizeof(uint);
	memcpy(&num_game_objects, cursor, bytes);
	cursor += bytes;

	std::vector<GameObject*> gameObjects_buffer;
	for (uint i = 0; i < num_game_objects; i++)
	{
		cursor += ExportGameObject(cursor, gameObjects_buffer);
	}
	return false;
}

bool ModuleLoader::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}
