#include "UITime.h"
#include "ModuleTime.h"
#include "Application.h"
#include "GameObject.h"

UITime::UITime(Application * app) : UIElement(app)
{
	name = "Time";
}

UITime::~UITime()
{
}

void UITime::Draw()
{
	if (ImGui::Begin(name.c_str(), &show_window))
	{
		if (App->time->GetState() != GAME_STATE::STATE_RUNNING)
		{
			if (ImGui::Button("Play"))
			{
				App->time->SetState(GAME_STATE::STATE_RUNNING);
			}
		}
		else
		{
			ImGui::Text("Play");
		}
		
		ImGui::SameLine();

		if (App->time->GetState() != GAME_STATE::STATE_PAUSED)
		{
			if (ImGui::Button("Pause"))
			{
				App->time->SetState(GAME_STATE::STATE_PAUSED);
			}
		}
		else
		{
			ImGui::Text("Pause");
		}

		ImGui::SameLine();

		if (App->time->GetState() != GAME_STATE::STATE_STOPPED)
		{
			if (ImGui::Button("Stop"))
			{
				App->time->SetState(GAME_STATE::STATE_STOPPED);
			}
		}
		else
		{
			ImGui::Text("Stop");
		}
		ImGui::SameLine();

		if (ImGui::Button("Tick"))
		{
			App->time->Tick(App->dt);
		}

		switch (App->time->GetState())
		{
		case GAME_STATE::STATE_RUNNING:
			ImGui::Text("Running");
			break;
		case GAME_STATE::STATE_PAUSED:
			ImGui::Text("Paused");
			break;
		case GAME_STATE::STATE_STOPPED:
			ImGui::Text("Stopped");
			break;
		}
		ImGui::SameLine();
		ImGui::Text("Real clock: %i", App->time->real_clock.Read());
		ImGui::SameLine();
		ImGui::Text("Game clock: %i", App->time->game_clock.Read());

		ImGui::SliderFloat("Game speed", &App->time->gameSpeed, 0.01f, 15.0f);

		ImGui::Text("Current app dt: %f", App->dt);
	}

	ImGui::End();
}
