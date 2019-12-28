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
		if(res->scriptTable["Start"].isFunction())
			res->scriptTable["Start"]();
	}
}

bool ComponentScript::UpdateComponent(float dt)
{
	bool ret = false;
	if (ResourceScript* res = (ResourceScript*)_app->resource->getResource(resource_uuid))
	{
		if (res->scriptTable["Update"].isFunction())
			res->scriptTable["Update"]();

		ret = true;
	}
	return ret;
}

void ComponentScript::AssignResourceUUID(uint uuid)
{
	resource_uuid = uuid;
	if (ResourceScript* res = (ResourceScript*)_app->resource->getResource(uuid))
		res->increaseAmountLoaded();
	else
		LOG("Trying to assign an unexisting resource");
}


ComponentScript::~ComponentScript()
{
}
