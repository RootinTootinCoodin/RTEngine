#ifndef __MODULE_LOADER_H__
#define __MODULE_LOADER_H__

#include "Module.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
class Component;
class ComponentMaterial;
class ComponentMesh;
class GameObject;
struct ILinfo;


class ModuleLoader :
	public Module
{
public:
	ModuleLoader(Application* parent, bool start_enabled = true);
	~ModuleLoader();

	bool CleanUp() override;

	bool FileReceived(std::string path);
	bool LoadFBX(std::string& path, std::string& name);
	bool LoadAiNodesRecursively(aiNode* node, const aiScene* scene,GameObject* parent, std::string& path, math::float4x4 cumulative_transform);
	bool LoadTexture(std::string& path, ComponentMaterial* material = nullptr);
	void LoadTransform(aiNode* node, GameObject* game_object);
	
	bool LoadMaterial(ILinfo& il_img_info, std::string& path, std::string& name, ComponentMaterial* material);

	void LoadVertices(ComponentMesh* _mesh, aiMesh* m);
	void LoadUVS(ComponentMesh* _mesh, aiMesh*m);
	void LoadNormals(ComponentMesh* _mesh, aiMesh* m);
	void LoadMeshTexture(ComponentMaterial* _mesh, aiMaterial* material, std::string& path);
	bool LoadMeshFaces(ComponentMesh* _mesh, aiMesh* m);
	GameObject* LoadMesh(aiMesh* m, GameObject* new_model, const aiScene* scene, std::string& path, std::string optional_name = std::string());

	bool SaveTextureAsDDS(std::string& name);
	bool ImportMesh(aiMesh* mesh);
	bool ExportMesh(ComponentMesh* mesh, char* buffer);

	bool ExportScene();
	bool ImportScene();

	bool ExportGameObject(GameObject* go, JSON_Object* go_json);
	bool ImportGameObject(); 

	bool ExportComponent(Component* component, JSON_Object* component_json);
	bool ImportComponent();
	//TODO: Import functions to binary
	//bool ImportGameobject(GameObject* game_object,char* buffer);
	//bool ImportComponent(Component* component, char*buffer);

	//TODO: Export functions to binary
	//uint ExportGameObject(char* buffer, std::vector<GameObject*> gameObjects_buffer);
	//uint ExportComponent(uint uuid, GameObject* go,char* buffer);
	//bool ExportScene(char* buffer);


private:
public:
private:
	aiLogStream stream;
};

#endif

