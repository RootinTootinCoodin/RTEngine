#include "ComponentMaterial.h"
#include "ResourceMaterial.h"
#include "ModuleResource.h"
#include "Application.h"


ComponentMaterial::ComponentMaterial()
{
}

ComponentMaterial::ComponentMaterial(GameObject * parent) : Component(MATERIAL, parent)
{
}


ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::AssignResourceUUID(uint uuid)
{
	resource_uuid = uuid;
	if (ResourceMaterial* res = (ResourceMaterial*)_app->resource->getResource(uuid))
		res->increaseAmountLoaded();
	else
		LOG("Trying to assign an unexisting resource");

}

