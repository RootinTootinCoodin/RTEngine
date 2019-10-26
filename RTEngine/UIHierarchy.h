#ifndef __UI_HIERARCHY_H__
#define __UI_HIERARCHY_H__

#include "UIElement.h"
class UIHierarchy :
	public UIElement
{
public:
	UIHierarchy(Application* app);
	~UIHierarchy();

	void Draw();
};

#endif // !__UI_HIERARCHY_H__

