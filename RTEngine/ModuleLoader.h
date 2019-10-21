#pragma once
#include "Module.h"



class ModuleLoader :
	public Module
{
public:
	ModuleLoader(Application* parent, bool start_enabled = true);
	~ModuleLoader();

	bool CleanUp();

	bool FileReceived(std::string path);
	bool LoadFBX(std::string path);
	bool LoadTexture(std::string path);
};

