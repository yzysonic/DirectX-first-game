#include "Time.h"
#include "Window.h"

#pragma comment (lib, "winmm.lib")

Timer*	Time::s_pFrameTimer;
Timer*	Time::s_pFPSTimer;
DWORD	Time::s_FrameError;
DWORD	Time::s_FrameCount;
float	Time::s_DeltaTime;
int		Time::s_fps;

void Time::Init()
{
	timeBeginPeriod(1); // •ª‰ð”\‚ðÝ’è
	s_pFrameTimer = new Timer(1.0f/SystemParameters::FPS);
	s_pFPSTimer = new Timer(1.0f);
	s_FrameError = 0;
	s_FrameCount = 0;
}

void Time::Uninit()
{
	SafeDelete(s_pFrameTimer);
	SafeDelete(s_pFPSTimer);
}

float Time::DeltaTime()
{
	return s_DeltaTime;
}

int Time::FPS()
{
	return s_fps;
}

Timer::Timer(float interval)
{
	this->startTime = timeGetTime();
	this->interval = (DWORD)(interval * 1000);
}

void Timer::reset()
{
	this->startTime = timeGetTime();
}

float Timer::elapsedTime()
{
	return (timeGetTime() - this->startTime)/1000.0f;
}

bool Timer::timeUp()
{
	if ((timeGetTime() - this->startTime) >= this->interval)
		return true;
	else
		return false;
}


void Time::FramerateControl()
{
	static DWORD elapsed, sleep, _elapsed, _error;

	// fpsŒv‘ª
	s_FrameCount++;
	elapsed = timeGetTime() - s_pFPSTimer->startTime;
	//if (elapsed >= g_pFPSTimer->interval)
	if (s_FrameCount == 30)
	{
		//g_fps = g_FrameCount / (g_pFPSTimer->interval / 1000.0f);
		s_fps = (int)(s_FrameCount / s_pFPSTimer->elapsedTime());
		s_FrameCount = 0;
		s_pFPSTimer->reset();

#ifdef _DEBUG
		char s[256];
		sprintf_s(s, "%s FPS:%d", SystemParameters::windowTitle, s_fps);
		SetWindowText(Window::GetHWnd(), s);
		//sprintf_s(s, "%d\n", elapsed);
		//OutputDebugString(s);
#endif

	}

	// fps‡‚í‚¹
	elapsed = timeGetTime() - s_pFrameTimer->startTime;
	//sleep = g_pFrameTimer->interval - min(elapsed + g_FrameError, g_pFrameTimer->interval);
	sleep = s_pFrameTimer->interval - min(elapsed, s_pFrameTimer->interval);
	if (sleep > 0)
	{
		Sleep(sleep);
		//_elapsed = elapsed;
		elapsed = (timeGetTime() - s_pFrameTimer->startTime);
		//g_FrameError = max(elapsed - g_pFrameTimer->interval, 0);
	}
	//else
	//	g_FrameError = 0;
	
	//g_FrameError = max(elapsed - g_pFrameTimer->interval, 0);
	s_DeltaTime = min(elapsed / 1000.0f, (1.0f / SystemParameters::FPS)*3);

	s_pFrameTimer->reset();

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
//	t1 = (DWORD)(1000.f / SystemParameters::FPS*(g_FrameCount));
//	sleep = t1 - min(t2, t1);
//	if (sleep > 0)
//	{
//		Sleep(sleep);
//	}
//
//	delta = (timeGetTime() - g_pFrameTimer->startTime);
//	Timer_Reset(g_pFrameTimer);
//}

FrameTimer::FrameTimer(float interval)
{
	this->elapsed = 0.0f;
	this->interval = interval;
}

void FrameTimer::Step(void)
{
	this->elapsed += Time::DeltaTime();
}

void FrameTimer::Reset(void)
{
	this->elapsed = 0.0f;
}

void FrameTimer::Reset(float interval)
{
	this->interval = interval;
	Reset();
}

float FrameTimer::Progress(void)
{
	return this->elapsed / this->interval;
}

bool FrameTimer::TimeUp(void)
{
	return this->elapsed >= this->interval;
}

void FrameTimer::operator++(int)
{
	Step();
}
