#ifndef __MODULE_RESORUCE_H__
#define __MODULE_RESOURCE_H__

#include "Module.h"
#include <map>
class Res;
enum  ResourceType {
	RES_MESH,
	RES_TEXTURE
};
class ModuleResourceManager :
	public Module
{
public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();

	bool Init(JSON_Object* config) override;

	Res* createNewResource(ResourceType type, uint uuid = 0);
	Res* getResource(uint uuid);
private:
	std::map<uint, Res*> resource_map;
};

#endif

