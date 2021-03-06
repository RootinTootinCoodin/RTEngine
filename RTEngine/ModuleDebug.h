#ifndef __MODULE_DEBUG_H__
#define __MODULE_DEBUG_H__

#include "Module.h"

struct par_shapes_mesh_s;
class ComponentMesh;
class GameObject;

enum OPERATION;

enum primitiveType
{
	PLANE,
	CUBE,
	SPHERE,
	HEMISPHERE,
	CYLINDER,
	CONE,
	KLEIN,
	TREFOIL
};

class ModuleDebug : public Module
{
public:
	ModuleDebug(Application* app, bool start_enabled = true);
	~ModuleDebug();

	bool Init(JSON_Object* config) override;
	bool CleanUp() override;

	void DrawNormals(ComponentMesh* mesh);

	void DrawAABB(AABB &aabb)const;

	void DrawFrustum(Frustum &frustum)const;

	void CreatePrimitive(par_shapes_mesh_s* data, char* name, primitiveType type);

private:


public:

	OPERATION operationType;
	bool usingGuizmo = false;

private:


};

#endif // !__MODULE_DEBUG_H__
