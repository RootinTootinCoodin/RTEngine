#ifndef __MODULE_DEBUG_H__
#define __MODULE_DEBUG_H__

#include "Module.h"

struct par_shapes_mesh_s;
class ComponentMesh;

class ModuleDebug : public Module
{
public:
	ModuleDebug(Application* app, bool start_enabled = true);
	~ModuleDebug();

	bool Init(JSON_Object* config);
	bool CleanUp();

	void DrawNormals(ComponentMesh* mesh);

	void DrawAABB(AABB &aabb)const;

private:


public:

	void CreatePrimitive(par_shapes_mesh_s* data, char* name);
	void RenderNormals(par_shapes_mesh_s* data);

private:


};

#endif // !__MODULE_DEBUG_H__
