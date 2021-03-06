#include "ModuleImGui.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "UIScene.h"
#include "UITests.h"
#include "UIConsole.h"
#include "UIAbout.h"
#include "UIConfiguration.h"
#include "UIInspector.h"
#include "UIHierarchy.h"
#include "UIAssets.h"
#include "UITime.h"
#include "UITextEditor.h"

#include <fstream>

ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Imgui";
}


ModuleImGui::~ModuleImGui()
{
}

bool ModuleImGui::Init(JSON_Object* config)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	scene = new UIScene(App);
	tests = new UITests(App);
	console = new UIConsole(App);
	about = new UIAbout(App);
	configuration = new UIConfiguration(App);
	inspector = new UIInspector(App);
	hierarchy = new UIHierarchy(App);
	assets = new UIAssets(App);
	time = new UITime(App);
	texted = new UITextEditor(App);

	UI.push_back((UIElement*)scene);
	UI.push_back((UIElement*)tests);
	UI.push_back((UIElement*)console);
	UI.push_back((UIElement*)about);
	UI.push_back((UIElement*)configuration);
	UI.push_back((UIElement*)inspector);
	UI.push_back((UIElement*)hierarchy);
	UI.push_back((UIElement*)assets);
	UI.push_back((UIElement*)time);
	UI.push_back((UIElement*)texted);

	uiFonts[IMGUI_DEFAULT] = io.Fonts->AddFontDefault();
	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{

	update_status ret = UPDATE_CONTINUE;
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.WantCaptureKeyboard = true;
	io.WantCaptureMouse = true;
	io.WantTextInput = true;

	CreateDockspace();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Close"))
			{
				ret = UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("Show demo", "S", &show_demo_window);

			if (ImGui::MenuItem("Documentation"))
				App->RequestBrowser("https://github.com/RootinTootinCoodin/RTEngine/wiki");

			if (ImGui::MenuItem("Download latest version"))
				App->RequestBrowser("https://github.com/RootinTootinCoodin/RTEngine/releases");

			if (ImGui::MenuItem("Report a bug"))
				App->RequestBrowser("https://github.com/RootinTootinCoodin/RTEngine/issues");

			if (ImGui::MenuItem("About"))
				about->SetActive(true);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			for (auto item = UI.begin(); item != UI.end(); item++)
			{
				if ((*item)->name.c_str() != "About")
				ImGui::MenuItem((*item)->name.c_str(), ".", &(*item)->show_window);
			}

			ImGui::EndMenu();
		}

	}

	for (auto item = UI.begin(); item != UI.end(); item++)
	{
		if((*item)->show_window)
			(*item)->Draw();
	}


	if (show_demo_window)
		ImGui::ShowDemoWindow();
	return ret;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	
	ImGui::EndMainMenuBar();
	ImGui::End();

	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	for (auto item = UI.begin(); item != UI.end(); item++)
	{
		RELEASE(*item);
	}
	return true;
}

void ModuleImGui::CreateDockspace()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", &dockspace_active, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);
	
	ImGuiIO& io = ImGui::GetIO();
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
}
