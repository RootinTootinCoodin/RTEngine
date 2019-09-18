#include "ModuleImGui.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"



ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleImGui::~ModuleImGui()
{
}

bool ModuleImGui::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	//ImGui::StyleColorsDark();
	//ImVec2 main_viewport_pos = ImGui::GetMainViewport()->Pos;
	//ImGui::SetNextWindowPos(ImVec2(main_viewport_pos.x + 650, main_viewport_pos.y + 20), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui::NewFrame();
	if(ImGui::BeginMainMenuBar())
	{ }
	return UPDATE_CONTINUE;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	
	ImGui::EndMainMenuBar();
	ImGui::Render();
	return UPDATE_CONTINUE;
}
