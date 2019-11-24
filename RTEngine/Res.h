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

	uint getAmountLoaded() const { return amount_loaded; };

	std::string GetOriginalFile()const { return original_file; };
	void SetOriginalFile(std::string file) { original_file = file; };
	ResourceType getType()const { return type; };

protected:
	uint uuid = 0;
	std::string original_file = "";
	std::string imported_file = "";

	ResourceType type;
	uint amount_loaded = 1;
};
#endif // !__RES_H__


