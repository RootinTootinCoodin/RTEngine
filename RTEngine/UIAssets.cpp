#include "UIAssets.h"
#include "Application.h"
#include "FileSystem.h"
#include "ModuleLoader.h"


UIAssets::UIAssets(Application * app) : UIElement(app)
{
	name = "Assets";

	FileSystem::DiscoverFiles("Assets", &file_arr, &dir_arr);
	current_dir = "Assets";
	parent_dir = "...";
}


UIAssets::~UIAssets()
{
}

void UIAssets::Draw()
{
	file_arr.clear();
	dir_arr.clear();

	FileSystem::DiscoverFiles(current_dir.data(), &file_arr, &dir_arr);

	if (ImGui::Begin(name.c_str(), &show_window))
	{
		ImGui::Text(current_dir.data());
		ImGui::Separator();

		if (current_dir.compare("Assets") != 0)
		{
			if (ImGui::Selectable("...", false, ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (ImGui::IsMouseDoubleClicked(0))
					current_dir = parent_dir.data();
			}
		}


		for (int i = 0; i < dir_arr.size(); i++)
		{
			std::size_t found = dir_arr[i].find_last_of("/\\");
			std::string filename = dir_arr[i].substr(found);
			if (ImGui::Selectable(filename.data(), false, ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (ImGui::IsMouseDoubleClicked(0))
					current_dir = dir_arr[i].data();
			}
		}

		//if (current_dir.compare("Assets") != 0)
		//{
			std::size_t found = current_dir.find_last_of("/\\");

			parent_dir = current_dir.substr(0, found);
		//}

		for (int i = 0; i < file_arr.size(); i++)
		{
			std::size_t found = file_arr[i].find_last_of("/\\");
			std::string filename = file_arr[i].substr(found + 1);
			if (ImGui::Button(filename.data()))
			{
				std::string extension = filename;
				FileSystem::getExtension(extension);
				if (extension == SCENE_EXTENSION)
				{
					_app->loader->ImportSceneOrModel(filename, true);
				}
				else if (MODEL_EXTENSIONS(extension))
				{
					std::string path;
					std::string library_model = filename;
					FileSystem::removeExtension(library_model);
					library_model += MODEL_EXTENSION;
					if(FileSystem::FindInDirectory(LIBRARY_MODELS_FOLDER, library_model.c_str(), path))
						_app->loader->ImportSceneOrModel(filename, false);
					else
					{
						std::string path =current_dir +"\\"+ filename;
						std::string name = filename;
						FileSystem::removeExtension(name);
						if (_app->loader->LoadFBX(path, name))
						{
							_app->loader->ImportSceneOrModel(name + MODEL_EXTENSION, false);
						}

					}
				}
			}
		}
	}
	ImGui::End();
}
