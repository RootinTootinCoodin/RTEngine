#include "UITextEditor.h"
#include <iostream>
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleScripting.h"
#include "ModuleImGui.h"
#include "FileSystem.h"

#include <fstream>

UITextEditor::UITextEditor(Application * app) : UIElement(app)
{
	name = "About";
	show_window = false;
}

UITextEditor::~UITextEditor()
{
}

void UITextEditor::InitializeScriptEditor()
{
	scriptEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Wren());

	//TextEditor::ErrorMarkers markers;
	//markers.insert(std::make_pair<int, std::string>(20, "Example error here:\nInclude file not found: \"TextEditor.h\""));
	//markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
	//script_editor.SetErrorMarkers(markers);

	scriptPath = "";

	std::ifstream t(scriptPath.c_str());
	if (t.good())
	{
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		scriptEditor.SetText(str);
	}
}

void UITextEditor::Draw()
{
	std::vector<std::string> files;
	FileSystem::DiscoverFiles(ASSETS_SCRIPTS_FOLDER, &files, nullptr);
	ImGui::PushFont(App->ImGui->uiFonts[IMGUI_DEFAULT]);

	auto cpos = scriptEditor.GetCursorPosition();

	if (ImGui::Begin(name.c_str(), &show_window, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar))
	{
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					auto textToSave = scriptEditor.GetText();
					//App->fs.SetFileString(scriptPath.c_str(), textToSave.c_str());
				}
				if (ImGui::MenuItem("Quit", "Alt-F4")) {
					// Exit or something
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				bool ro = scriptEditor.IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					scriptEditor.SetReadOnly(ro);
				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && scriptEditor.CanUndo()))
					scriptEditor.Undo();
				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && scriptEditor.CanRedo()))
					scriptEditor.Redo();

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, scriptEditor.HasSelection()))
					scriptEditor.Copy();
				if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && scriptEditor.HasSelection()))
					scriptEditor.Cut();
				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && scriptEditor.HasSelection()))
					scriptEditor.Delete();
				if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
					scriptEditor.Paste();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
					scriptEditor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(scriptEditor.GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
					scriptEditor.SetPalette(TextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light palette"))
					scriptEditor.SetPalette(TextEditor::GetLightPalette());
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, scriptEditor.GetTotalLines(),
			scriptEditor.IsOverwrite() ? "Ovr" : "Ins",
			scriptEditor.CanUndo() ? "*" : " ",
			scriptEditor.GetLanguageDefinition().mName.c_str(), scriptPath.c_str());

		TextEditor::CommandKeys c_keys;
		c_keys.ctrl = (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RCTRL) == KEY_REPEAT);
		c_keys._X = App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN;
		c_keys._Y = App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN;
		c_keys._Z = App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN;
		c_keys._C = App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN;
		c_keys._V = App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN;

		scriptEditor.Render("TextEditor", App->ImGui->uiFonts[IMGUI_DEFAULT], c_keys);
		ImGui::PopFont();
		ImGui::End();
	}
	ImGui::End();
}