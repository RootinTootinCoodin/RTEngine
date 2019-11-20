#ifndef __MODULE_TIME_H__
#define __MODULE_TIME_H__

#include "Module.h"

class Application;

class ModuleTime : public Module
{
public:

	ModuleTime(Application* app, bool start_enabled = true);

	virtual ~ModuleTime();

	bool Init(JSON_Object* config);
	bool CleanUp();

	void Save(JSON_Object* config);

	void SetTitle(std::string& title);


};

#endif // __MODULE_TIME_H__