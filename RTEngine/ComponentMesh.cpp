#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ModuleResource.h"
#include "Application.h"


ComponentMesh::ComponentMesh()
{
}

ComponentMesh::ComponentMesh(GameObject * parent) : Component(MESH,parent)
{
}


ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::ComponentCleanUp()
{
	if (ResourceMesh* res = (ResourceMesh*)_app->resource->getResource(resource_uuid))
		res->decreaseAmountLoaded();
}

void ComponentMesh::AssignResourceUUID(uint uuid)
{
	resource_uuid = uuid;
	if (ResourceMesh* res = (ResourceMesh*)_app->resource->getResource(uuid))
		res->increaseAmountLoaded();
	else
		LOG("Trying to assign an unexisting resource");
}

