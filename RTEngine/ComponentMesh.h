#pragma once
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
	std::string mesh_name;

	uint id_index = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint num_vertices = 0;
	float* vertices = nullptr;

	float* colors = nullptr;

	uint id_uvs = 0;
	uint num_uvs = 0;
	float* uvs = nullptr;

	ComponentMaterial* material = nullptr;
private:

};

