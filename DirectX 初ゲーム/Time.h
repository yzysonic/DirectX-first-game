#pragma once

#include"main.h"

typedef struct _Timer
{
	DWORD startTime;
	DWORD interval;
}Timer;

void InitTime();
void UninitTime();
float GetDeltaTime();
int GetFPS();

Timer* newTimer(float interval = 0.0f);
void Timer_Reset(Timer* timer);
float Timer_ElapsedTime(Timer* timer);
bool Timer_TimeUp(Timer* timer);

void FramerateControl();