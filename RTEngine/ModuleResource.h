#ifndef __MODULE_RESORUCE_H__
#define __MODULE_RESOURCE_H__

#include "Module.h"
#include <map>
class Res;
class ResourceMaterial;
enum  ResourceType {
	RES_MESH,
	RES_TEXTURE,
	RES_SCRIPT
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

	uint CheckIfFileIsLoaded(std::string path);

	std::vector<ResourceMaterial*> getMaterials();

private:
	std::map<uint, Res*> resource_map;
};

#endif

