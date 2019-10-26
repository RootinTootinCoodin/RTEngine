#include "UIAbout.h"
#include <iostream>
#include "Globals.h"
#include "Application.h"
#include "Assimp/include/version.h"
#include "DevIL/include/il.h"
#include "PhysFS/include/physfs.h"

UIAbout::UIAbout(Application * app) : UIElement(app)
{
	name = "About";
	show_window = false;
}

UIAbout::~UIAbout()
{
}

void UIAbout::Draw()
{
	if (ImGui::Begin(name.c_str(), &show_window))
	{
		ImGui::Text("RTEngine\nNow that's what I call a rootin' tootin' engine, partner.\nBy Lorién Portella and Joel Cabaco.\n");
		ImGui::Text("3rd Party Libraries Used:");

		// SDL
		SDL_version linked;
		SDL_version compiled;

		SDL_GetVersion(&linked);
		SDL_VERSION(&compiled);

		ImGui::Text("  - SDL");
		ImGui::SameLine();
		ImGui::Text("Compiled %d.%d.%d", compiled.major, compiled.minor, compiled.patch);
		ImGui::SameLine();
		ImGui::Text("Linked %d.%d.%d", compiled.major, compiled.minor, compiled.patch);
		ImGui::SameLine();
		if (ImGui::Button("Site"))
			App->RequestBrowser("https://www.libsdl.org/");

		// ImGui
		ImGui::Text("  - ImGui");
		ImGui::SameLine();
		ImGui::Text(IMGUI_VERSION);
		ImGui::SameLine();
		if (ImGui::Button("Site"))
			App->RequestBrowser("https://github.com/ocornut/imgui");

		// GLew
		ImGui::Text("  - GLew (OpenGL Extension Wrangler)");
		ImGui::SameLine();
		ImGui::Text((char*)glewGetString(GLEW_VERSION));
		ImGui::SameLine();
		if (ImGui::Button("Site"))
			App->RequestBrowser("http://glew.sourceforge.net/");

		// Parson
		ImGui::Text("  - Parson");
		ImGui::SameLine();
		if (ImGui::Button("Site"))
			App->RequestBrowser("https://kgabis.github.io/parson/");

		// Assimp
		ImGui::Text("  - Assimp");
		ImGui::SameLine();
		ImGui::Text("%d.%d.%d", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
		ImGui::SameLine();
		if (ImGui::Button("Site"))
			App->RequestBrowser("http://www.assimp.org/");

		// Par_shapes
		ImGui::Text("  - par_shapes");
		ImGui::SameLine();
		if (ImGui::Button("Site"))
			App->RequestBrowser("https://github.com/prideout/par");

		// DevIL
		ImGui::Text("  - DevIL");
		ImGui::SameLine();
		ImGui::Text("%d", IL_VERSION);
		ImGui::SameLine();
		if (ImGui::Button("Site"))
			App->RequestBrowser("http://openil.sourceforge.net/");

		// PhysFS
		ImGui::Text("  - PhysFS");
		ImGui::SameLine();
		ImGui::Text("%d.%d.%d", PHYSFS_VER_MAJOR, PHYSFS_VER_MINOR, PHYSFS_VER_PATCH);
		ImGui::SameLine();
		if (ImGui::Button("Site"))
			App->RequestBrowser("https://icculus.org/physfs/");

		ImGui::Text("MIT License\n\nCopyright(c) 2019 RootinTootinCoodin\n\nPermission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files(the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and/or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions :\n\nThe above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.");

		ImGui::Text(text.c_str());
	}
	ImGui::End();
}