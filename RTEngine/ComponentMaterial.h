#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"

class texture;

class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial();
	ComponentMaterial(GameObject* parent);

	~ComponentMaterial();

	void CopyTextureToThis(texture* texture);
	std::string name;
	uint id_texture;
	uint width;
	uint height;
	uint depth;
	uint bpp;
};

#endif // !__COMPONENT_MATERIAL_H__
