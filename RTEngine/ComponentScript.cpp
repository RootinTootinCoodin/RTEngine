#include "ComponentScript.h"
#include "ModuleResource.h"
#include "ResourceScript.h"
#include "Application.h"
#include "ModuleScripting.h"


ComponentScript::ComponentScript()
{
}

ComponentScript::ComponentScript(GameObject * parent) 
{
	//Weird bug
	this->type = SCRIPT;
	this->gameObject = parent;
	uuid = Generate_UUID();
}

void ComponentScript::ComponentStart()
{
	if (ResourceScript* res = (ResourceScript*)_app->resource->getResource(uuid))
	{
		if (res->compiled)
		{
			if (res->scriptTable["Start"].isFunction())
			{
				res->scriptTable["Start"]();
			}
		}
	}
}

bool ComponentScript::UpdateComponent(float dt)
{
	bool ret = false;
	if (ResourceScript* res = (ResourceScript*)_app->resource->getResource(resource_uuid))
	{
		if (res->compiled)
		{
			if (res->scriptTable["Update"].isFunction())
			{
				res->scriptTable["Update"]();
				ret = true;
			}
		}
	}
	return ret;
}

void ComponentScript::AssignResourceUUID(uint uuid)
{
	if (resource_uuid != 0)
	{
		_app->resource->DeleteResource(resource_uuid);
	}
	resource_uuid = uuid;
	if (ResourceScript* res = (ResourceScript*)_app->resource->getResource(uuid))
		res->increaseAmountLoaded();
	else
		LOG("Trying to assign an unexisting resource");
}


ComponentScript::~ComponentScript()
{
}
