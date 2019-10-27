#include "ComponentMaterial.h"



ComponentMaterial::ComponentMaterial()
{
}

ComponentMaterial::ComponentMaterial(GameObject * parent) : Component(MATERIAL, parent)
{
}


ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::CopyTextureToThis(texture * texture)
{
	name = texture->name;
	path = texture->path;
	id_texture = texture->id_texture;
	width = texture->width;
	height = texture->height;
	depth = texture->depth;
	bpp = texture->bpp;
}
