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
	name = "Text Editor";
	show_window = false;
	scriptEditor = new TextEditor();
	scriptEditor->SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
	scriptEditor->SetText("--Open a script by clicking it in the inspector or the assets window");
}

UITextEditor::~UITextEditor()
{
}

void UITextEditor::Draw()
{
	auto cpos = scriptEditor->GetCursorPosition();

	if (ImGui::Begin(name.c_str(), &show_window, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar))
	{
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save") && scriptPath != ".")
				{
					std::string file = scriptEditor->GetText();
					file.erase(file.find_last_of("d") + 1, 100);
					std::vector<char> cstr(file.c_str(), file.c_str() + file.size() + 1);
					FileSystem::ExportBuffer(cstr.data(), cstr.size() - 1, scriptName.c_str(), ASSETS_SCRIPTS, ".lua");
					App->scripting->RecompileAllScriptsWithPath(scriptPath);
				}

				if (ImGui::MenuItem("Quit", "Alt-F4")) {
					show_window = false;
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				bool ro = scriptEditor->IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					scriptEditor->SetReadOnly(ro);
				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && scriptEditor->CanUndo()))
					scriptEditor->Undo();
				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && scriptEditor->CanRedo()))
					scriptEditor->Redo();

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, scriptEditor->HasSelection()))
					scriptEditor->Copy();
				if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && scriptEditor->HasSelection()))
					scriptEditor->Cut();
				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && scriptEditor->HasSelection()))
					scriptEditor->Delete();
				if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
					scriptEditor->Paste();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
					scriptEditor->SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(scriptEditor->GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
					scriptEditor->SetPalette(TextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light palette"))
					scriptEditor->SetPalette(TextEditor::GetLightPalette());
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, scriptEditor->GetTotalLines(),
			scriptEditor->IsOverwrite() ? "Ovr" : "Ins",
			scriptEditor->CanUndo() ? "*" : " ",
			scriptEditor->GetLanguageDefinition().mName.c_str());

		TextEditor::CommandKeys c_keys;
		c_keys.ctrl = (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RCTRL) == KEY_REPEAT);
		c_keys._X = App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN;
		c_keys._Y = App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN;
		c_keys._Z = App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN;
		c_keys._C = App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN;
		c_keys._V = App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN;

		scriptEditor->Render("TextEditor", App->ImGui->uiFonts[IMGUI_DEFAULT], c_keys);

		ImGui::End();
	}

//	ImGui::PopFont();
}

void UITextEditor::SetNewScript(std::string path)
{
	char* buffer = FileSystem::ImportFile(path.c_str());
	std::string file = buffer;
	file.erase(file.find_last_of("d") + 1, 100);

	scriptEditor->SetText(file);

	scriptPath = path;
	scriptName = path;
	FileSystem::getFileNameFromPath(scriptName);
}
