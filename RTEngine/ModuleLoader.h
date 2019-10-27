#ifndef __MODULE_LOADER_H__
#define __MODULE_LOADER_H__

#include "Module.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
class ComponentMaterial;
class ComponentMesh;
class GameObject;

class ModuleLoader :
	public Module
{
public:
	ModuleLoader(Application* parent, bool start_enabled = true);
	~ModuleLoader();

	bool CleanUp() override;

	bool FileReceived(std::string& path);
	bool LoadFBX(std::string& path, std::string& name);
	bool LoadTexture(std::string& path, ComponentMaterial* material = nullptr);

	void LoadVertices(ComponentMesh* _mesh, aiMesh* m);
	void LoadUVS(ComponentMesh* _mesh, aiMesh*m);
	void LoadNormals(ComponentMesh* _mesh, aiMesh* m);
	void LoadMeshTexture(ComponentMaterial* _mesh, aiMaterial* material, std::string& path);
	void LoadMeshFaces(ComponentMesh* _mesh, aiMesh* m);
	void LoadMesh(aiMesh* m, GameObject* new_model, const aiScene* scene, std::string& path);

private:
public:
private:
	aiLogStream stream;
};

#endif

