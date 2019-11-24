#ifndef __RESOURCE_MATERIAL_H__
#define __RESOURCE_MATERIAL_H__
#include "Res.h"
class ResourceMaterial :
	public Res
{
public:
	ResourceMaterial(uint uuid);
	~ResourceMaterial();

	std::string name;
	std::string path;
	uint id_texture;
	uint width;
	uint height;
	uint depth;
	uint bpp;
};


#endif // !1

