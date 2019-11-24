#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__


#include <vector>
#include <map>
#include "Module.h"

struct model;
struct par_shapes_mesh_s;
class GameObject;
class Tree;

class ModuleScene :
	public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init(JSON_Object* config);
	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;

	void CreateTree();

	void GenerateCheckerTexture();
	void DefaultTexture();
	void Draw();
	void DrawGrid(int halfsize);
	void DrawAxis();
	bool drawaxis = true;
	bool showedges = false;
	bool draw_aabb = false;
	bool draw_normals = false;
	bool drawEditorFrustum = false;
	bool frustCulling = false;
	float axis_x = 0.0f;
	float axis_y = 0.0f;
	float axis_z = 0.0f;
	int gridsize = 10;

	GLubyte checkImage[100][100][4];

	bool save = false;
	int checker_size = 100; 
	std::string scene_name = "Name";
	GameObject* root = nullptr;
	GameObject* selected_go = nullptr;
	Tree* quadtree = nullptr;
	AABB treeArea;
	float3 treeSize = { 150.0f, 50.0f, 150.0f };
	bool debugQuad = false;
	int bucket = 1;
};

#endif // !__MODULE_SCENE_H__


