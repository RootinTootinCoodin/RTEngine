#ifndef __UI_INSPECTOR_H__
#define __UI_INSPECTOR_H__

#include "UIElement.h"

struct model;
class UIInspector :
	public UIElement
{
public:
	UIInspector(Application* app);
	~UIInspector();

	void Draw();

	void DrawModelInfo(model* _model);
};

#endif // !__UI_INSPECTOR_H__

