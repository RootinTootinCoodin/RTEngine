#ifndef __RESOURCE_MES_H__
#define __RESOURCE_MES_H__



#include "Res.h"
#include "Globals.h"
class ResourceMesh :
	public Res
{
public:
	ResourceMesh(uint uuid);
	~ResourceMesh();

	//Mesh
	bool HasTextureCoords() { return uvs != nullptr; };
	bool HasNormals() { return normals != nullptr; };
	bool HasVertexColors() { return colors != nullptr; };

	uint id_index = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint num_vertices = 0;
	float* vertices = nullptr; //num vertices * 3 in size

	float* colors = nullptr;

	float* normals = nullptr; //num vertices in size

	float* uvs = nullptr; //num
};

#endif // !__RESOURCE_MES_H__