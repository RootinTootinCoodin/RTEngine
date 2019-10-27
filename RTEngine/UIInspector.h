#ifndef __UI_INSPECTOR_H__
#define __UI_INSPECTOR_H__

#include "UIElement.h"

struct model;
class GameObject;
class ComponentTransform;
class ComponentMesh;

class UIInspector :
	public UIElement
{
public:
	UIInspector(Application* app);
	~UIInspector();

	void Draw();
	void DrawGameObjectInfo(GameObject* gameobject);
	void DrawTransformInfo(ComponentTransform* transform);
	void DrawMeshInfo(ComponentMesh* mesh);


};

#endif // !__UI_INSPECTOR_H__

