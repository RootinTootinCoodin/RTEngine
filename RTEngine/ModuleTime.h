#ifndef __MODULE_TIME_H__
#define __MODULE_TIME_H__

#include "Module.h"
#include "Timer.h"

class Application;

enum GAME_STATE
{
	STATE_STOPPED,
	STATE_PAUSED,
	STATE_RUNNING
};

class ModuleTime : public Module
{

public:

	ModuleTime(Application* app, bool start_enabled = true);

	virtual ~ModuleTime();

	bool Init(JSON_Object* config);
	update_status Update(float dt);
	bool CleanUp();

	void Save(JSON_Object* config);

	void SetTitle(std::string& title);

	void SetState(GAME_STATE state);
	GAME_STATE GetState();
	void Tick(float dt);

	void SaveStartState();
	void SetToStart();

	Timer real_clock;
	Timer game_clock;

	bool stop = true;

	float game_dt;
	float gameSpeed = 1;

private:

	GAME_STATE gameState = STATE_STOPPED;
};

#endif // __MODULE_TIME_H__