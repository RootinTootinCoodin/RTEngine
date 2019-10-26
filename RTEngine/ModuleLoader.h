#ifndef __MODULE_LOADER_H__
#define __MODULE_LOADER_H__

#include "Module.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

class ModuleLoader :
	public Module
{
public:
	ModuleLoader(Application* parent, bool start_enabled = true);
	~ModuleLoader();

	bool CleanUp();

	bool FileReceived(std::string path);
	bool LoadFBX(std::string path, std::string name);
	bool LoadTexture(std::string path, std::string name);

private:
public:
private:
	aiLogStream stream;
};

#endif

