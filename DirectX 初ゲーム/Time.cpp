#include"Time.h"

#pragma comment (lib, "winmm.lib")

Timer* g_pFrameTimer;
Timer* g_pFPSTimer;
DWORD g_FrameError;
DWORD g_FrameCount;
int g_fps;
float g_DeltaTime;

void InitTime()
{
	timeBeginPeriod(1); // •ª‰ð”\‚ðÝ’è
	g_pFrameTimer = newTimer(1.0f/MAX_FPS);
	g_pFPSTimer = newTimer(1.0f);
	g_FrameError = 0;
	g_FrameCount = 0;
}

void UninitTime()
{
	SafeDelete(g_pFrameTimer);
	SafeDelete(g_pFPSTimer);
}

float GetDeltaTime()
{
	return g_DeltaTime;
}

int GetFPS()
{
	return g_fps;
}

Timer * newTimer(float interval)
{
	Timer* thiz = New(Timer);
	thiz->startTime = timeGetTime();
	thiz->interval = (DWORD)(interval * 1000);
	return thiz;
}

void Timer_Reset(Timer * timer)
{
	timer->startTime = timeGetTime();
}

float Timer_ElapsedTime(Timer * timer)
{
	return (timeGetTime() - timer->startTime)/1000.0f;
}

bool Timer_TimeUp(Timer * timer)
{
	if ((timeGetTime() - timer->startTime) >= timer->interval)
		return true;
	else
		return false;
}


void FramerateControl()
{
	static DWORD elapsed, sleep, _elapsed, _error;

	// fpsŒv‘ª
	g_FrameCount++;
	elapsed = timeGetTime() - g_pFPSTimer->startTime;
	//if (elapsed >= g_pFPSTimer->interval)
	if (g_FrameCount >= 30)
	{
		//g_fps = g_FrameCount / (g_pFPSTimer->interval / 1000.0f);
		g_fps = (int)(g_FrameCount / Timer_ElapsedTime(g_pFPSTimer));
		g_FrameCount = 0;
		Timer_Reset(g_pFPSTimer);
	}

	// fps‡‚í‚¹
	elapsed = timeGetTime() - g_pFrameTimer->startTime;
	//sleep = g_pFrameTimer->interval - min(elapsed + g_FrameError, g_pFrameTimer->interval);
	sleep = g_pFrameTimer->interval - min(elapsed, g_pFrameTimer->interval);
	if (sleep > 0)
	{
		Sleep(sleep);
		_elapsed = elapsed;
		elapsed = (timeGetTime() - g_pFrameTimer->startTime);
		g_FrameError = max(elapsed - g_pFrameTimer->interval, 0);
	}
	else
		g_FrameError = 0;
	
	//g_FrameError = max(elapsed - g_pFrameTimer->interval, 0);
	g_DeltaTime = elapsed / 1000.0f;

	Timer_Reset(g_pFrameTimer);

#ifdef _DEBUG
	char s[256];
	sprintf_s(s, "%s FPS:%d", WINDOW_TITLE, GetFPS());
	SetWindowText(GetHWnd(), s);
	//sprintf_s(s, "%d\n", elapsed);
	//OutputDebugString(s);
#endif
}


//void FramerateControl()
//{
//	static DWORD elapsed, sleep, delta = 0, t1, t2 = 0;
//	static float fps;
//
//	t2 += delta;
//
//	// fpsŒv‘ª
//	g_FrameCount++;
//	if (g_FrameCount%20 == 0)
//	{
//		g_fps = g_FrameCount / (t2/1000.0f);
//		g_FrameCount = 0;
//		t2 = 0;
//	}
//
//	// fps‡‚í‚¹
//	t1 = (DWORD)(1000.f / MAX_FPS*(g_FrameCount));
//	sleep = t1 - min(t2, t1);
//	if (sleep > 0)
//	{
//		Sleep(sleep);
//	}
//
//	delta = (timeGetTime() - g_pFrameTimer->startTime);
//	Timer_Reset(g_pFrameTimer);
//}

