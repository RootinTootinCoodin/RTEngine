#include "Globals.h"
#include "Application.h"
#include "ModuleTime.h"
#include "ModuleScene.h"
#include "ModuleLoader.h"
#include "Component.h"
#include "GameObject.h"

#include <map>

ModuleTime::ModuleTime(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Time";
}

ModuleTime::~ModuleTime()
{
}

bool ModuleTime::Init(JSON_Object * config)
{
	return true;
}

update_status ModuleTime::Update(float dt)
{
	if (gameState == STATE_RUNNING)
		Tick(dt * gameSpeed);

	if (gameState == STATE_PAUSED || gameState == STATE_STOPPED)
		Tick(0.0f);

	return UPDATE_CONTINUE;
}

bool ModuleTime::CleanUp()
{
	return false;
}

void ModuleTime::Save(JSON_Object * config)
{
}

void ModuleTime::SetTitle(std::string & title)
{
}

void ModuleTime::SetState(GAME_STATE state)
{
	switch (state)
	{
	case STATE_RUNNING:
		if (stop)
			SaveStartState();
		if (real_clock.Read() == 0)
			real_clock.Start();
		game_clock.Start();
		gameState = STATE_RUNNING;
		stop = false;
		break;
	case STATE_PAUSED:
		game_clock.Pause();
		gameState = STATE_PAUSED;
		break;
	case STATE_STOPPED:
		SetToStart();
		real_clock.Stop();
		game_clock.Stop();
		gameState = STATE_STOPPED;
		stop = true;
		break;
	}
}

GAME_STATE ModuleTime::GetState()
{
	return gameState;
}

void ModuleTime::Tick(float dt)
{
	std::vector<GameObject*> gameObjects;
	App->scene->root->RecursiveGetChildren(&gameObjects);

	for (auto item = gameObjects.begin(); item != gameObjects.end(); item++)
	{
		if ((*item)->active)
		{
			std::map<uint, Component*> components = (*item)->GetComponentList();

			for (auto comp = components.begin(); comp != components.end(); comp++)
			{
				(*comp).second->UpdateComponent(dt);
			}
		}
	}
}

void ModuleTime::SaveStartState()
{
	_app->scene->save = true;
}

void ModuleTime::SetToStart()
{
	_app->loader->ImportSceneOrModel(_app->scene->scene_name, true);

}

