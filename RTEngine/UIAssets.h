#ifndef __UI_ASSETS_H__
#define __UI_ASSETS_H__

#include "UIElement.h"
class UIAssets : public UIElement
{
public:
	UIAssets(Application* app);
	~UIAssets();

	void Draw();

	std::string text;

	std::vector<std::string> file_arr;
	std::vector<std::string> dir_arr;
};

#endif // !__UI_ASSETS_H__

