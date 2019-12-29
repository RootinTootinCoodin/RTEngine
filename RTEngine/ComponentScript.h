#pragma once
#include "Component.h"

class ComponentScript :
	public Component
{
public:
	ComponentScript();
	ComponentScript(GameObject* parent);

	void ComponentStart() override;
	bool UpdateComponent(float dt) override;

	void AssignResourceUUID(uint uuid);
	uint getResourceUUID()const { return resource_uuid; };

	std::vector<std::string*> varList;

	uint resource_uuid;
	~ComponentScript();
};

