#ifndef __UI_SCENE_H__
#define __UI_SCENE_H__

#include "UIElement.h"
#include "MathGeoLib/Math/float2.h"

class UIScene :
	public UIElement
{
public:
	UIScene(Application* App);
	~UIScene();

	void Draw();

	bool IsMouseHovering();
	void GetSceneDimensions();
	bool hovered;


};

#endif

