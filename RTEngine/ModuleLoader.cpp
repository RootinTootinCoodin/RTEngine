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
		App->scene->CreateTree();
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

	math::float4x4 node_transform = float4x4::FromTRS(pos,rot,scale);
	math::float4x4 transform = cumulative_transform * node_transform;

	if (node->mNumMeshes > 0)
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			if(node->mName.length >0)
				go = LoadMesh(scene->mMeshes[node->mMeshes[i]], parent, scene, path,node->mName.C_Str());
			else
				go = LoadMesh(scene->mMeshes[node->mMeshes[i]], parent, scene, path,node->mParent->mName.C_Str());

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

	std::string path2 = ".\\";
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

GameObject* ModuleLoader::LoadMesh(aiMesh * m, GameObject* new_model, const aiScene* scene, std::string& path, std::string optional_name)
{
	GameObject* mesh_gameobject = nullptr;

	if(m->mName.length > 0)
		mesh_gameobject = new_model->AddChildren(m->mName.C_Str());
	else
		mesh_gameobject = new_model->AddChildren(optional_name);


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

bool ModuleLoader::ImportMesh(ComponentMesh* mesh)
{
		//GOD BLESS C++ 
		uint uvs = mesh->HasTextureCoords();
		uint normals = mesh->HasNormals();
		uint colors = mesh->HasVertexColors();

		//Header 
		uint ranges[5] = { mesh->num_vertices,mesh->num_indices ,uvs,normals,colors};
		//Calculate the size of the file
		uint size = sizeof(ranges) + sizeof(uint) * mesh->num_indices + sizeof(float) * mesh->num_vertices * 3;
		if (uvs)
			size += sizeof(float) * mesh->num_vertices * 2;
		
		if (normals)
			size += sizeof(float) * mesh->num_vertices * 3;

		if (colors)
			size += sizeof(float) * mesh->num_vertices * 4;

		//Data: File, Cursor: Pointer to where we are writing in the file
		char* data = new char[size];
		char* cursor = data;

		//Store the ranges first
		uint bytes = sizeof(ranges);
		memcpy(cursor, ranges, bytes);

		//Store Vertices
		cursor += bytes; //We "move" the cursor a distance equal to the memory copied previously;
		bytes = sizeof(float) * mesh->num_vertices * 3;
		memcpy(cursor, mesh->vertices, bytes);

		//Store Indices

		cursor += bytes;
		bytes = sizeof(uint) * mesh->num_indices;
		memcpy(cursor, mesh->indices, bytes);
		cursor += bytes;

		if (uvs)
		{
			bytes = sizeof(uint) * mesh->num_vertices * 2;
			memcpy(cursor, mesh->uvs, bytes);
			cursor += bytes;
		}



		if (normals)
		{
			bytes = sizeof(float) * mesh->num_vertices * 3;
			memcpy(cursor, mesh->normals, bytes);
			cursor += bytes;
		}

		if (colors)
		{
			bytes = sizeof(float) * mesh->num_vertices * 4;
			memcpy(cursor, mesh->colors, bytes);
		}

		return FileSystem::ExportBuffer(data, size, std::to_string(mesh->GetUUID()).c_str(), LIBRARY_MESHES, ".meshprtnr");
}

//bool ModuleLoader::ImportGameobject(GameObject * go,char* pre_buffer)
//{
//	//size order: name, uuid, parent uuid, component uuids, active, static, 
//	uint num_components = go->GetNumComponents();
//	uint size = sizeof(go->GetName()) + sizeof(uint) + sizeof(uint) + sizeof(uint)*num_components + sizeof(bool) * 2;
//	char* data = new char[size];
//	char* cursor = data;
//
//
//	uint bytes = sizeof(go->GetName());
//	memcpy(cursor, &go->GetName(), bytes);
//
//	//uuid
//	cursor += bytes;
//	bytes = sizeof(uint);
//	uint uuid = go->GetUUID();
//	memcpy(cursor,&uuid, bytes);
//
//	//parent uuid
//	cursor += bytes;
//	bytes = sizeof(uint);
//	uuid = go->GetParentUUID();
//	memcpy(cursor, &uuid, bytes);
//
//
//
//	//component UUIDS
//	std::map<uint, Component*> components = go->GetComponentList();
//
//	uint component_size = components.size();
//	cursor += bytes;
//	bytes = sizeof(uint);
//	memcpy(cursor, &component_size, bytes);
//
//	for (auto item = components.begin(); item != components.end(); item++)
//	{
//		cursor += bytes;
//		bytes = sizeof(uint);
//		uint uuid = (*item).first;
//		memcpy(cursor, &uuid, bytes);
//	}
//
//	//active
//	cursor += bytes;
//	bytes = sizeof(bool);
//	memcpy(cursor, &go->active, bytes);
//
//	cursor += bytes;
//	bytes = sizeof(bool);
//	memcpy(cursor, &go->is_static, bytes);
//
//
//	return false;
//}
//
//bool ModuleLoader::ImportComponent(Component * component, char * buffer)
//{
//	return false;
//}

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
		mesh->normals = new float[mesh->num_vertices * 3];
		memcpy(mesh->normals, cursor, bytes);
		cursor += bytes;
	}
	if (ranges[4])
	{
		bytes = sizeof(float) * mesh->num_vertices * 4;
		mesh->colors = new float[mesh->num_vertices * 4];
		memcpy(mesh->colors, cursor, bytes);
		cursor += bytes;
	}


	return true;
}

bool ModuleLoader::ExportScene()
{
	bool ret = false;
	JSON_Value* file = json_value_init_object();	
	JSON_Value* objects_array = json_value_init_array();	
	json_object_set_value(json_object(file), "Scene", objects_array);

	std::vector<GameObject*> gameObjects;
	App->scene->root->RecursiveGetChildren(&gameObjects);

	for (auto item = gameObjects.begin(); item != gameObjects.end(); item++)
	{
		JSON_Value* json_go = json_value_init_object();	
		ret = ExportGameObject((*item), json_object(json_go));
		json_array_append_value(json_array(objects_array), json_go);

	}
	if (ret)
	{
		std::string path_buffer;
		FileSystem::FormFullPath(path_buffer, App->scene->scene_name.c_str(), ASSETS_SCENES,".westscene");
		json_serialize_to_file(file, path_buffer.c_str());
	}
	else
		LOG("Unable to save the scene");

	return ret;
}

bool ModuleLoader::ImportScene(std::string& path)
{
	bool ret = false;
	App->scene->root->RecursiveDeleteGameobject();
	//the path received is just the name with the extension
	std::string name = path.substr(0, path.find_last_of("."));

	FileSystem::FormFullPath(path, name.c_str(), ASSETS_SCENES, ".westscene");
	JSON_Value* file = json_parse_file(path.c_str());
	if (file)
	{
		ret = true;
		std::map<uint, GameObject*> scene_gameobjects;
		JSON_Array* json_scene = json_object_get_array(json_object(file), "Scene");
		for (uint i = 0; i < json_array_get_count(json_scene); i++)
		{
			JSON_Object* json_go = json_array_get_object(json_scene, i);
			if (GameObject* go = ImportGameObject(json_go))
				scene_gameobjects.insert({ go->GetUUID(), go });
		}

		for (auto item = scene_gameobjects.begin(); item != scene_gameobjects.end(); item++)
		{
			if ((*item).second->GetUUID() != 0)
			{
				if ((*item).second->parent_uuid != 0)
				{
					(*item).second->SetParent(scene_gameobjects[(*item).second->parent_uuid]);
					scene_gameobjects[(*item).second->parent_uuid]->AddChildren((*item).second);
				}
				else
				{
					(*item).second->SetParent(App->scene->root);
					App->scene->root->AddChildren((*item).second);
				}
			}
		}
		LinkMeshesWithMaterials(scene_gameobjects);

	}
	else
	{
		LOG("Could not open scene %s", path.c_str());
	}

	return ret;
}

bool ModuleLoader::ExportGameObject(GameObject * go, JSON_Object * go_json)
{
	bool ret = true;
	json_object_set_string(go_json, "name", go->GetName().c_str());
	json_object_set_number(go_json, "UUID", go->GetUUID());
	if(go->GetParent()) //Root does not have parent
		json_object_set_number(go_json, "parent", go->GetParentUUID());
	else
		json_object_set_number(go_json, "parent", 1);

	JSON_Value* component_array = json_value_init_array(); 

	std::map<uint, Component*> components = go->GetComponentList();
	for (auto item = components.begin(); item != components.end(); item++) {
		JSON_Value* component_json = json_value_init_object(); 
		ret = ExportComponent((*item).second, json_object(component_json));
		json_array_append_value(json_array(component_array), component_json);

		go->ParentRecalculateAABB();
	}

	json_object_set_value(go_json, "components", component_array);
	json_object_set_boolean(go_json, "static", go->is_static);
	json_object_set_boolean(go_json, "active", go->active);
	return ret;
}

GameObject* ModuleLoader::ImportGameObject(JSON_Object * json_go)
{
	bool ret = false;
	GameObject* go = new GameObject();
	go->SetName(json_object_get_string(json_go, "name"));
	go->SetUUID(json_object_get_number(json_go, "UUID"));
	go->SetParentUUID(json_object_get_number(json_go, "parent"));

	JSON_Array* component_array = json_object_get_array(json_go, "components");
	for (uint i = 0; i < json_array_get_count(component_array); i++)
	{
		ImportComponent(json_array_get_object(component_array, i),go);
	}

	return go;
}

bool ModuleLoader::ExportComponent(Component * component, JSON_Object * component_json)
{
	switch (component->GetComponentType())
	{
	case TRANSFORM:
	{
		ComponentTransform* transform = (ComponentTransform*)component;
		json_object_set_string(component_json, "component_type", "transform");

		float3 position = transform->getPos();
		json_object_set_number(component_json, "position_x", position.x);
		json_object_set_number(component_json, "position_y", position.y);
		json_object_set_number(component_json, "position_z", position.z);

		Quat rotation = transform->getRotationQuat();
		json_object_set_number(component_json, "quaternion_x", rotation.x);
		json_object_set_number(component_json, "quaternion_y", rotation.y);
		json_object_set_number(component_json, "quaternion_z", rotation.z);
		json_object_set_number(component_json, "quaternion_w", rotation.w);

		float3 scale = transform->getScale();
		json_object_set_number(component_json, "scale_x", scale.x);
		json_object_set_number(component_json, "scale_y", scale.y);
		json_object_set_number(component_json, "scale_z", scale.z);

		break;
	}
	case MESH:
	{
		json_object_set_string(component_json, "component_type", "mesh");
		ComponentMesh* mesh = (ComponentMesh*)component;
		ImportMesh(mesh);

		break;
	}
	case MATERIAL:
	{
		json_object_set_string(component_json, "component_type", "material");
		ComponentMaterial* material = (ComponentMaterial*)component;

		json_object_set_string(component_json, "name", material->name.c_str());


		break;
	}
	case CAMERA:
	{
		json_object_set_string(component_json, "component_type", "camera");
		break;
	}
	}
	json_object_set_number(component_json, "UUID", component->GetUUID());
	return true;
}

bool ModuleLoader::ImportComponent(JSON_Object * json_go, GameObject* go)
{
	bool ret = false;
	
	std::string component_type = json_object_get_string(json_go, "component_type");
	if (component_type == "transform")
	{
		ret = true;
		ComponentTransform* component = (ComponentTransform*)go->AddComponent(TRANSFORM);
		component->setUUID(json_object_get_number(json_go, "UUID"));

		float3 position;
		position.x = json_object_get_number(json_go, "position_x");
		position.y = json_object_get_number(json_go, "position_y");
		position.z = json_object_get_number(json_go, "position_z");
		component->setPos(position);

		Quat rotation;
		rotation.x = json_object_get_number(json_go, "quaternion_x");
		rotation.y = json_object_get_number(json_go, "quaternion_y");
		rotation.z = json_object_get_number(json_go, "quaternion_z");
		rotation.w = json_object_get_number(json_go, "quaternion_w");
		component->setRotation(rotation);

		float3 scale;
		scale.x = json_object_get_number(json_go, "scale_x");
		scale.y = json_object_get_number(json_go, "scale_y");
		scale.z = json_object_get_number(json_go, "scale_z");
		component->setScale(scale);
	}
	else if (component_type == "mesh")
	{
		std::string mesh_file;
		//THIS IS SO FUCKING UGLY
		std::string uuid = std::to_string(json_object_get_number(json_go, "UUID"));
		uuid = uuid.substr(0, uuid.find_last_of("."));
		FileSystem::FormFullPath(mesh_file, uuid.c_str(), LIBRARY_MESHES, ".meshprtnr");
		char* buffer = FileSystem::ImportFile(mesh_file.c_str());
		ComponentMesh* component = (ComponentMesh*)go->AddComponent(MESH);
		component->setUUID(json_object_get_number(json_go, "UUID"));
		ret = ExportMesh(component, buffer);
		App->renderer3D->GenerateBufferForMesh(component);
	}
	else if (component_type == "material")
	{
		std::string texture_file;
		FileSystem::FormFullPath(texture_file,json_object_get_string(json_go,"name"), LIBRARY_TEXTURES, ".dds");
		ComponentMaterial* component = (ComponentMaterial*)go->AddComponent(MATERIAL);
		component->setUUID(json_object_get_number(json_go, "UUID"));
		LoadTexture(texture_file,component);
	}
	else if (component_type == "camera")
	{
		ret = true;
	}
	return ret;
}

void ModuleLoader::LinkMeshesWithMaterials(std::map<uint, GameObject*> scene_gameobjects)
{
	for (auto item = scene_gameobjects.begin(); item != scene_gameobjects.end(); item++)
	{
		if (ComponentMesh* mesh = (ComponentMesh*)(*item).second->GetComponent(MESH))
		{
			if (ComponentMaterial* material = (ComponentMaterial*)(*item).second->GetComponent(MATERIAL))
			{
				mesh->material = material;
			}
		}
	}
}

//uint ModuleLoader::ExportGameObject(char * buffer, std::vector<GameObject*> gameObjects_buffer)
//{
//	GameObject* new_go = new GameObject();
//
//	char* cursor = buffer;
//
//	uint size_of_name = 0;
//	uint bytes = sizeof(uint);
//	memcpy(&size_of_name, cursor, bytes);
//	cursor += bytes;
//
//	bytes = size_of_name;
//	std::string name(cursor, size_of_name);
//	new_go->SetName(name);
//	cursor += bytes;
//
//	bytes = sizeof(uint);
//	uint uuid = 0;
//	memcpy(&uuid, cursor, bytes);
//	new_go->SetUUID(uuid);
//	cursor += bytes;
//
//	bytes = sizeof(uint);
//	uint puuid = 0;
//	memcpy(&puuid, cursor, bytes);
//	new_go->SetParentUUID(puuid);
//	cursor += bytes;
//
//	bytes = sizeof(uint);
//	uint num_of_components = 0;
//	memcpy(&num_of_components, cursor, bytes);
//	cursor += bytes;
//
//	uint comp_uuid = 0;
//	for (int i = 0; i < num_of_components; i++)
//	{
//		bytes = sizeof(uint);
//		memcpy(&comp_uuid, cursor, bytes);
//		cursor += bytes;
//		ExportComponent(comp_uuid, new_go,cursor);
//	}
//
//
//	return false;
//}
//
//uint ModuleLoader::ExportComponent(uint uuid, GameObject * go, char* buffer)
//{
//	return uint();
//}
//
//bool ModuleLoader::ExportScene(char * buffer)
//{
//	char* cursor = buffer;
//	uint num_game_objects;
//	uint bytes = sizeof(uint);
//	memcpy(&num_game_objects, cursor, bytes);
//	cursor += bytes;
//
//	std::vector<GameObject*> gameObjects_buffer;
//	for (uint i = 0; i < num_game_objects; i++)
//	{
//		cursor += ExportGameObject(cursor, gameObjects_buffer);
//	}
//	return false;
//}

bool ModuleLoader::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}
