#pragma once
#include <vector>
#include "Module.h"

struct mesh;

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

	void Draw();
	void DrawGrid(int halfsize);
	void DrawAxis();
	void DrawCubeDirectMode();
	void DrawCubeVertexArray();

	int gridsize = 10;
	uint index_id = 0;
	uint vertex_id = 0;

	mesh* model;
};

