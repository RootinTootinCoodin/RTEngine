#pragma once
#include "Module.h"

struct mesh
{
	uint id_index = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertex = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_uvs = 0;
	float* uvs = nullptr;
	bool has_texture = false;
};
class ModuleLoader :
	public Module
{
public:
	ModuleLoader(Application* parent, bool start_enabled = true);
	~ModuleLoader();

	bool CleanUp();

	bool LoadFBX(std::string path);
};

