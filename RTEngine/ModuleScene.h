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

	void GenerateCheckerTexture();
	void GenerateTexture(uint* texture, uint width, uint height);
	void Draw();
	void DrawGrid(int halfsize);
	void DrawAxis();
	void DrawCubeDirectMode();
	void DrawCubeVertexArray();

	int gridsize = 10;
	uint index_id = 0;
	uint vertex_id = 0;

	GLubyte checkImage[100][100][4];
	uint id_image = 0;


	std::vector<mesh*> model;
	bool model_loaded = false;
	int checker_size = 100; 
};

