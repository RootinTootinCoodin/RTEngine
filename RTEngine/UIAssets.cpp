#include "UIAssets.h"
#include "Application.h"
#include "FileSystem.h"


UIAssets::UIAssets(Application * app) : UIElement(app)
{
	name = "Assets";

	FileSystem::DiscoverFiles("Assets//", &file_arr, &dir_arr);
}


UIAssets::~UIAssets()
{
}

void UIAssets::Draw()
{
	file_arr.clear();
	dir_arr.clear();

	FileSystem::DiscoverFiles("Assets//", &file_arr, &dir_arr);

	if (ImGui::Begin(name.c_str(), &show_window))
	{
		ImGui::Text("Assets//");
		ImGui::Separator();

		for (int i = 0; i < dir_arr.size(); i++)
		{
			ImGui::Text(dir_arr[i].data());
		}

		for (int i = 0; i < file_arr.size(); i++)
		{
			ImGui::Text(file_arr[i].data());
		}
	}
	ImGui::End();
}
