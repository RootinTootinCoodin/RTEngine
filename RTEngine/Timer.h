#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();
	void Pause();
	void Unpause();

	bool running;

	Uint32 Read();
	Uint32 ReadSec();

private:

	bool	paused;
	Uint32	started_at;
	Uint32	paused_at;
	Uint32	accumulated = 0;
};

#endif //__TIMER_H__