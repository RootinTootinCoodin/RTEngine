#ifndef __UI_SCENE_H__
#define __UI_SCENE_H__

#include "UIElement.h"
class UIScene :
	public UIElement
{
public:
	UIScene(Application* App);
	~UIScene();

	void Draw();
};

#endif

