#ifndef __RESOURCE_SCRIPT_H__
#define __RESOURCE_SCRIPT_H__
#include "Res.h"

#include "ModuleScripting.h"

class ResourceScript :
	public Res
{
public:
	ResourceScript(uint uuid);
	~ResourceScript();

	LuaRef scriptTable = 0;
};

#endif

