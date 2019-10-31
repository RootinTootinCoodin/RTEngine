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
	float* vertices = nullptr; //num vertices * 3 in size

	float* colors = nullptr;

	float* normals = nullptr; //num vertices in size

	float* uvs = nullptr; //num

	ComponentMaterial* material = nullptr;

	bool draw_normals = false;
private:

};

#endif

