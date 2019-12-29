#ifndef __UI_INSPECTOR_H__
#define __UI_INSPECTOR_H__

#include "UIElement.h"

struct model;
class GameObject;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;
class ComponentScript;

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
	void DrawCameraInfo(ComponentCamera* camera);
	void DrawScriptInfo(ComponentScript* script);

	void SelectScript(GameObject* go);
	void NewScript(char* script_name);

};

#endif // !__UI_INSPECTOR_H__

