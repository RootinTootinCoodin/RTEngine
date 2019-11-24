#ifndef __RES_H__
#define __RES_H__
#include "Globals.h"

enum ResourceType;
class Res
{
public:
	Res(uint uuid, ResourceType type);
	~Res();

	uint GetUUID() { return uuid; };

	void increaseAmountLoaded() { amount_loaded++; };
	void decreaseAmountLoaded() { amount_loaded--; };

	uint getAmountLoaded() { return amount_loaded; };
protected:
	uint uuid = 0;
	std::string original_file;
	std::string imported_file;

	ResourceType type;
	uint amount_loaded = 0;
};
#endif // !__RES_H__


