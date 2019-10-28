#include "UIAssets.h"
#include "Application.h"
#include "ModuleFileSystem.h"


UIAssets::UIAssets(Application * app) : UIElement(app)
{
	name = "Assets";

	App->fileSystem->DiscoverFiles(App->fileSystem->GetWritePath(), file_arr, dir_arr);
}


UIAssets::~UIAssets()
{
}

void UIAssets::Draw()
{
	file_arr.clear();
	dir_arr.clear();
	std::string path = App->fileSystem->GetWritePath();
	path += ASSETS_FOLDER;
	App->fileSystem->DiscoverFiles(path.data(), file_arr, dir_arr);

	if (ImGui::Begin(name.c_str(), &show_window))
	{
		ImGui::Text(path.data());
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
