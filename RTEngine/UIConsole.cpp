#include "UIConsole.h"


UIConsole::UIConsole(Application* app) : UIElement(app)

{
	name = "Console";
}


UIConsole::~UIConsole()
{
}

void UIConsole::Draw()
{
	ImGui::Begin(name.c_str(), &show_window);
	ImGui::End();
}

