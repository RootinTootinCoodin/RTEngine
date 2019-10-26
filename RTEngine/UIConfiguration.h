#ifndef __UI_CONFIGURATION_H__
#define __UI_CONFIGURATION_H__


#include "UIElement.h"
#include "SDL/include/SDL.h"

class UIConfiguration :
	public UIElement
{
public:
	UIConfiguration(Application* app);
	~UIConfiguration();

	void Draw();

	int display_fps = 60;
};

#endif // !__UI_CONFIGURATION_H__

