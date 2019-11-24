// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
{
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	accumulated = 0;
}

void Timer::Pause()
{
	running = false;
	paused_at = SDL_GetTicks();
	accumulated += paused_at - started_at;
}

void Timer::Unpause()
{
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - started_at + accumulated;
	}
	else
	{
		return accumulated;
	}
}

Uint32 Timer::ReadSec()
{
	if (running == true)
	{
		return (SDL_GetTicks() - started_at)/1000;
	}
	else
	{
		return accumulated / 1000;
	}
}


