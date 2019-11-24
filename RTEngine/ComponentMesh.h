#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__
#include "Component.h"


#include <vector>

class ComponentMaterial;
struct Color;

class ComponentMesh :
	public Component
{
public:
	ComponentMesh();
	ComponentMesh(GameObject* parent);
	~ComponentMesh();

	void ComponentCleanUp() override;
	
	void AssignResourceUUID(uint uuid);
	uint getResourceUUID()const { return resource_uuid; };


private:
	uint resource_uuid = 0;

public:

	ComponentMaterial* material = nullptr;

	bool draw_normals = false;
private:

};

#endif

