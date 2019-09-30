#include "UIConfiguration.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

UIConfiguration::UIConfiguration(Application* app) : UIElement(app)
{
	name = "Configuration";
}


UIConfiguration::~UIConfiguration()
{
}

void UIConfiguration::Draw()
{
	if (ImGui::Begin(name.c_str(), &show_window))
	{
		if (ImGui::CollapsingHeader("Application")) // 1 - Application
		{
			if (ImGui::InputText("App Name", App->window->title, 25, ImGuiInputTextFlags_EnterReturnsTrue))
				App->window->UpdateTitle();

			ImGui::InputText("Organization Name", App->organization, 25, ImGuiInputTextFlags_EnterReturnsTrue);

			ImGui::SliderInt("Framerate limit", &App->fps_limit_display, 0, 240);
			ImGui::Text("Current limit: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", App->fps_limit);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "FPS");
			char fpstitle[25];
			sprintf_s(fpstitle, 25, "Framerate (Avg): %.1f", App->GetAvgFPS());
			char lastframefps[25];
			sprintf_s(lastframefps, 25, "Last Frame %.1f", App->GetCurrentFPS());
			ImGui::PlotHistogram(lastframefps, App->fps_arr.data(), App->fps_arr.size(), 0, fpstitle, 0.0f, 180.0f, ImVec2(310, 100));

			sprintf_s(fpstitle, 25, "Miliseconds %f", App->GetCurrentMS());
			ImGui::PlotHistogram("", App->ms_arr.data(), App->ms_arr.size(), 0, fpstitle, 4.0f, 120.0f, ImVec2(310, 100));
		}

		if (ImGui::CollapsingHeader("Window")) // 2 - Window
		{
			if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0.0f, 1.0f))
				App->window->UpdateBrightness();
			if (ImGui::DragInt("Width", &App->window->width))
				App->window->UpdateSize();
			if (ImGui::DragInt("Height", &App->window->height))
				App->window->UpdateSize();

			ImGui::Text("Display refresh rate: ");
			ImGui::SameLine();
			App->window->UpdateRefreshRate();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", App->window->refresh_rate);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Hz");

			if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
			{
				App->window->UpdateWindowMode();
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &App->window->resizable))
			{
				App->window->UpdateWindowMode();
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Only applies after application restart.");
			ImGui::SameLine();
			if (ImGui::Checkbox("Borderless", &App->window->borderless))
			{
				App->window->UpdateWindowMode();
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Fullscreen Desktop", &App->window->full_desktop))
			{
				App->window->UpdateWindowMode();
			}
		}

		if (ImGui::CollapsingHeader("Hardware")) // 3 - Hardware
		{
			SDL_version linked;
			SDL_version compiled;

			SDL_GetVersion(&linked);
			SDL_VERSION(&compiled);

			ImGui::Text("SDL Compile Version: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d.%d.%d", compiled.major, compiled.minor, compiled.patch);

			ImGui::Text("SDL Linked Version: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d.%d.%d", linked.major, linked.minor, linked.patch);

			ImGui::Separator();

			ImGui::Text("CPUs: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", SDL_GetCPUCount());
			ImGui::SameLine();
			ImGui::Text(" Cache:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " %dkb", SDL_GetCPUCacheLineSize());

			float systemram_gb = (float)SDL_GetSystemRAM() / 1024;

			ImGui::Text("System RAM: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2fGB", systemram_gb);

			ImGui::Text("Caps: ");
			ImGui::SameLine();
			if (SDL_HasAVX())
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AVX ");
			ImGui::SameLine();
			if (SDL_HasAVX2())
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AVX2 ");
			ImGui::SameLine();
			if (SDL_HasAltiVec())
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AltiVec ");
			ImGui::SameLine();
			if (SDL_HasMMX())
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MMX ");
			ImGui::SameLine();
			if (SDL_HasRDTSC())
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "RDTSC ");
			ImGui::SameLine();
			if (SDL_HasSSE())
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE ");
			ImGui::SameLine();
			if (SDL_HasSSE2())
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE2 ");
			ImGui::SameLine();
			if (SDL_HasSSE3())
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE3 ");
			ImGui::SameLine();
			if (SDL_HasSSE41())
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE4.1 ");
			ImGui::SameLine();
			if (SDL_HasSSE42())
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE4.2 ");

			ImGui::Separator();

			// GPU stuff
		}


	}
	ImGui::End();
}
