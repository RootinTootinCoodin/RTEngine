#include "ModuleResource.h"
#include "Res.h"
#include "ResourceMesh.h"

ModuleResourceManager::ModuleResourceManager(Application * app, bool start_enabled) : Module(app,start_enabled)
{
	name = "resource manager";
}


ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Init(JSON_Object * config)
{
	return true;
}

Res * ModuleResourceManager::createNewResource(ResourceType type, uint _uuid)
{
	Res* ret = getResource(_uuid);
	if (!ret)
	{
		uint uuid;
		if (_uuid == 0)
			uuid = Generate_UUID();
		else
			uuid = _uuid;

		switch (type)
		{
		case RES_MESH:
			ret = new ResourceMesh(uuid);
			break;
		case RES_TEXTURE:

			break;
		}
		resource_map[uuid] = ret;
	}
	else
	{
		LOG("Trying to create an already existing resource, use getResource instead");
		ret = nullptr;
	}
	return ret;
}

Res * ModuleResourceManager::getResource(uint uuid)
{
	Res* ret = nullptr;
	auto item = resource_map.find(uuid);
	if (item != resource_map.end())
		ret = (*item).second;
		
	return ret;
}
