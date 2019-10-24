#pragma once
#include <vector>
#include <map>
#include "Module.h"

struct model;
struct par_shapes_mesh_s;
class GameObject;

class ModuleScene :
	public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init(JSON_Object* config);
	bool Start();
	update_status Update(float dt);
	std::vector<float> vert_array;
	std::vector<uint> index_array;

	void GenerateCheckerTexture();
	void Draw();
	void DrawGrid(int halfsize);
	void DrawAxis();
	void DrawCubeDirectMode();

	bool drawaxis = true;
	int gridsize = 10;
	uint index_id = 0;
	uint vertex_id = 0;

	GLubyte checkImage[100][100][4];
	uint id_image = 0;


	std::vector<model*> models;
	uint current_model_index = 0;

	std::vector<uint*> textures;
	uint current_texture_index = 0;

	std::vector<par_shapes_mesh_s*> primitive;
	bool model_loaded = false;
	int checker_size = 100; 

	GameObject* root;
};

