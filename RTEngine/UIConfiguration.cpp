#include "UIConfiguration.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ModuleDebug.h"
#include "ModuleCamera3D.h"
#include "ComponentCamera.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes/par_shapes.h"

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
			static char app_title[120];
			strcpy_s(app_title, 120, App->window->title.c_str());
			if (ImGui::InputText("App Name",app_title, 25))
				App->window->UpdateTitle(app_title);

			static char org_name[120];
			strcpy_s(org_name, 120, App->organization.c_str());

			if (ImGui::InputText("Organization Name", org_name, 25))
				App->organization = org_name;

			ImGui::SliderInt("Framerate limit", &App->fps_limit_display, 0, 240);
			ImGui::Text("Current limit: ");
			ImGui::SameLine();
			if (App->fps_limit_display > 0)
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", App->fps_limit_display);
			else
			{
				App->window->UpdateRefreshRate();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", App->window->refresh_rate);
			}
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "FPS");
			if (App->window->refresh_rate < App->fps_limit_display)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "!!!");
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("The current limit is above the display's refresh rate.\nHaving VSync on will limit the framerate to the display's refresh rate.");
			}
			char fpstitle[25];
			sprintf_s(fpstitle, 25, "Framerate (Avg): %.1f", App->GetAvgFPS());
			char lastframefps[25];
			sprintf_s(lastframefps, 25, "Last Frame %.1f", App->GetCurrentFPS());


			ImGui::SliderInt("Sample count", &display_fps, 1, 400);
			ImGui::PlotHistogram(lastframefps, App->fps_arr.data(), display_fps, 0, fpstitle, 0.0f, 180.0f, ImVec2(310, 100));

			sprintf_s(fpstitle, 25, "Miliseconds %f", App->GetCurrentMS());
			ImGui::PlotHistogram("", App->ms_arr.data(), display_fps, 0, fpstitle, 4.0f, 120.0f, ImVec2(310, 100));
		}

		if (ImGui::CollapsingHeader("Editor Camera"))
		{
			ImGui::SliderFloat("Mouse sensitivity", &App->camera->sensitivity, 0.0f, 10.0f);
			ImGui::SliderFloat("Speed", &App->camera->speed, 0.5f, 20.0f);
			ImGui::SliderFloat("Speed acceleration (shift)", &App->camera->speedmultiplier, 0.5f, 25.0f);
			ImGui::SliderFloat("Zoom speed", &App->camera->wheel, 0.5f, 150.0f);

			if (ImGui::DragFloat("Near plane distance", &App->camera->editorCamera->near_plane_distance))
			{
				App->camera->editorCamera->UpdateFrustum();
			}
			if (ImGui::DragFloat("Far plane distance", &App->camera->editorCamera->far_plane_distance))
			{
				App->camera->editorCamera->UpdateFrustum();
			}
			ImGui::Checkbox("Debug draw camera", &App->scene->drawEditorFrustum);
		}

		if (ImGui::CollapsingHeader("Render"))
		{
			const char* cull_items[] = { "Back", "Front", "Back and front"};
			static int cull_item_current = 0;

			ImGui::Text("Culling Mode");
			ImGui::SameLine();
			if (ImGui::Combo("", &cull_item_current, cull_items, IM_ARRAYSIZE(cull_items)))
				App->renderer3D->UpdateFaceCullSetting(cull_item_current);

			if (ImGui::Checkbox("Face Culling", &App->renderer3D->cullface_enabled))
				App->renderer3D->SetFaceCull();

			if (ImGui::Checkbox("Wireframe only ", &App->renderer3D->wireframe_enabled))
				App->renderer3D->SetWireframe();
			ImGui::SameLine();
			ImGui::Checkbox("Wireframe", &App->scene->showedges);
			ImGui::Checkbox("Draw AABBs", &App->scene->draw_aabb);
			ImGui::Checkbox("Draw Normals", &App->scene->draw_normals);
			if (ImGui::Checkbox("Lighting  ", &App->renderer3D->lighting_enabled))
				App->renderer3D->SetLighting();
			ImGui::SameLine();
			if (ImGui::Checkbox("Color Material", &App->renderer3D->color_material_enabled))
				App->renderer3D->SetColorMaterial();
		
			if (ImGui::Checkbox("Depth Test", &App->renderer3D->depth_test_enabled))
				App->renderer3D->SetDepthTest();
			ImGui::SameLine();
			if (ImGui::Checkbox("2D Textures", &App->renderer3D->texture2D_enabled))
				App->renderer3D->SetTexture2D();

			ImGui::Separator();

			ImGui::Checkbox("Display Axis", &App->scene->drawaxis);
			ImGui::SameLine();
			if (ImGui::Button("Reset to origin"))
			{
				App->scene->axis_x = 0;
				App->scene->axis_y = 0;
				App->scene->axis_z = 0;
			}

			ImGui::DragFloat("Axis x", &App->scene->axis_x);
			ImGui::DragFloat("Axis y", &App->scene->axis_y);
			ImGui::DragFloat("Axis z", &App->scene->axis_z);
			ImGui::DragInt("Grid size", &App->scene->gridsize, 1.0f, 0);
		}

		if (ImGui::CollapsingHeader("Space partitioning"))
		{
			ImGui::Checkbox("Quadtree debug draw", &App->scene->debugQuad);
			ImGui::SliderInt("Node bucket", &App->scene->bucket, 1, 15);
		}

		if (ImGui::CollapsingHeader("Window")) // 2 - Window
		{
			if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0.0f, 1.0f))
				App->window->UpdateBrightness();
			ImGui::Text("Gamma:");

			if (ImGui::SliderInt("Red", &App->window->r, 0, 255))
				App->window->UpdateGamma();
			if (ImGui::SliderInt("Green", &App->window->g, 0, 255))
				App->window->UpdateGamma();
			if (ImGui::SliderInt("Blue", &App->window->b, 0, 255))
				App->window->UpdateGamma();

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
			ImGui::SameLine();
			ImGui::Text("Display index: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", SDL_GetWindowDisplayIndex(App->window->window));

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

			ImGui::Text("GPU Vendor: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), (char*)glGetString(GL_VENDOR));

			ImGui::Text("GPU Model: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), (char*)glGetString(GL_RENDERER));

			GLint dedicated_mem = 0;
			glGetIntegerv(0x9047,
				&dedicated_mem);

			GLint total_mem = 0;
			glGetIntegerv(0x9048,
				&total_mem);

			GLint curr_avail_mem = 0;
			glGetIntegerv(0x9049,
				&curr_avail_mem);

			GLint eviction_count = 0;
			glGetIntegerv(0x904A,
				&eviction_count);

			GLint evicted_mem = 0;
			glGetIntegerv(0x904B,
				&evicted_mem);

			dedicated_mem /= 1000;
			total_mem /= 1000;
			curr_avail_mem /= 1000;
			evicted_mem /= 1000;

			ImGui::Text("Total Available VRAM: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%dMb", total_mem);

			ImGui::Text("Current Available VRAM: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%dMb", curr_avail_mem);

			ImGui::Text("Dedicated VRAM: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%dMb", dedicated_mem);

			ImGui::Text("Eviction Count: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", eviction_count);

			ImGui::Text("Evicted VRAM: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%dMb", evicted_mem);

			// GPU stuff
		}

		if (ImGui::CollapsingHeader("Geometry"))
		{
			static int slices = 30;
			static int stacks = 30;

			ImGui::Text("Slices");
			ImGui::SameLine();
			ImGui::InputInt("", &slices);

			ImGui::Text("Stacks");
			ImGui::SameLine();
			ImGui::InputInt("", &stacks);

			if (ImGui::TreeNode("Basic Geometry"))
			{
				if (ImGui::Button("Create plane"))
				{
					App->debug->CreatePrimitive(par_shapes_create_plane(slices, stacks), "Plane",PLANE);
				}

				if (ImGui::Button("Create cube"))
				{
					App->debug->CreatePrimitive(par_shapes_create_3d_cube(), "Cube",CUBE);
				}

				if (ImGui::Button("Create sphere"))
				{
					App->debug->CreatePrimitive(par_shapes_create_parametric_sphere(slices, stacks), "Sphere",SPHERE);
				}

				if (ImGui::Button("Create hemisphere"))
				{
					App->debug->CreatePrimitive(par_shapes_create_hemisphere(slices, stacks), "Hemisphere",HEMISPHERE);
				}

				if (ImGui::Button("Create tube"))
				{
					App->debug->CreatePrimitive(par_shapes_create_cylinder(slices, stacks), "Cylinder",CYLINDER);
				}

				if (ImGui::Button("Create cone"))
				{
					App->debug->CreatePrimitive(par_shapes_create_cone(slices, stacks), "Cone",CONE);
				}

				ImGui::TreePop();
			}

			ImGui::Separator();
			
			if (ImGui::TreeNode("Test Geometry"))
			{
				if (ImGui::Button("Create Klein bottle"))
				{
					App->debug->CreatePrimitive(par_shapes_create_klein_bottle(slices, stacks), "Klein bottle",KLEIN);
				}

				if (ImGui::Button("Create trefoil knot"))
				{
					App->debug->CreatePrimitive(par_shapes_create_trefoil_knot(slices, stacks, 1), "Trefoil",TREFOIL);
				}
				ImGui::TreePop();
			}
		}

		ImGui::Separator();

		if(ImGui::Button("SaveConfig"))
		{
			App->SaveConfig();
		}
	}
	ImGui::End();
}
