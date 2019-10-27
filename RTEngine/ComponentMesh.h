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
private:
public:
	uint id_index = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint num_vertices = 0;
	float* vertices = nullptr;

	float* colors = nullptr;
	float* normals = nullptr;

	uint id_uvs = 0;
	uint num_uvs = 0;
	float* uvs = nullptr;

	ComponentMaterial* material = nullptr;

	bool draw_normals = false;
private:

};

#endif

