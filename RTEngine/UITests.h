#ifndef __UI_TESTS_H__
#define __UI_TESTS_H__


#include "UIElement.h"



class UITests :
	public UIElement
{
public:
	UITests(Application* App);
	~UITests();

	void Draw();
private:
public:
private:
	//pcg32 seed;
};

#endif // !__UI_TESTS_H

