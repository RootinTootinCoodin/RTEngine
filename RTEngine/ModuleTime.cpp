#include "Globals.h"
#include "Application.h"
#include "ModuleTime.h"
#include "ModuleScene.h"
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
					//(*comp)->UpdateComponent();
				}
			}
		}
	}


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
		gameState = STATE_RUNNING;
		break;
	case STATE_PAUSED:
		gameState = STATE_PAUSED;
		break;
	case STATE_STOPPED:
		gameState = STATE_STOPPED;
		break;
	}
}

GAME_STATE ModuleTime::GetState()
{
	return gameState;
}

void ModuleTime::Tick()
{
}

