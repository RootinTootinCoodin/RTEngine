#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__


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
	void DefaultTexture();
	void Draw();
	void DrawGrid(int halfsize);
	void DrawAxis();
	bool drawaxis = true;
	bool showedges = false;
	bool wireframe_when_selected_enabled = true;
	float axis_x = 0.0f;
	float axis_y = 0.0f;
	float axis_z = 0.0f;
	int gridsize = 10;
	uint index_id = 0;
	uint vertex_id = 0;

	GLubyte checkImage[100][100][4];
	uint id_image = 0;

	std::vector<model*> models;
	uint current_model_index = 0;

	std::vector<texture*> textures;
	uint current_texture_index = 0;

	std::vector<par_shapes_mesh_s*> primitive;
	int checker_size = 100; 

	GameObject* root = nullptr;
	GameObject* selected_go = nullptr;
};

#endif // !__MODULE_SCENE_H__


