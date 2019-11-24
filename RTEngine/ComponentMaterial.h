#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"


class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial();
	ComponentMaterial(GameObject* parent);

	~ComponentMaterial();

	void AssignResourceUUID(uint uuid);
	void GhostAssignResourceUUID(uint uuid) { resource_uuid = uuid; }; //So edgy
	uint getResourceUUID()const { return resource_uuid; };

private:
	uint resource_uuid = 0;


};

#endif // !__COMPONENT_MATERIAL_H__
