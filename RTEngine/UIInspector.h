#ifndef __UI_INSPECTOR_H__
#define __UI_INSPECTOR_H__

#include "UIElement.h"

struct model;
class GameObject;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;

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
	void DrawMaterialInfo(ComponentMaterial* material);



};

#endif // !__UI_INSPECTOR_H__

