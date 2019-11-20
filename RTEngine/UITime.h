#ifndef __UI_TIME_H__
#define __UI_TIME_H__

#include "UIElement.h"
class UITime : public UIElement
{
public:
	UITime(Application* app);
	~UITime();

	void Draw();
};

#endif // !__UI_TIME_H__