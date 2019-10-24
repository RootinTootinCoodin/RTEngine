#pragma once
#include "Module.h"

struct par_shapes_mesh_s;

class ModuleDebug : public Module
{
public:
	ModuleDebug(Application* app, bool start_enabled = true);
	~ModuleDebug();

	bool Init(JSON_Object* config);
	bool CleanUp();

private:


public:

	void CreatePrimitive(par_shapes_mesh_s* data, char* name);
	void RenderNormals(par_shapes_mesh_s* data);

private:


};