#pragma once
#include "Core.h"
#include "Singleton.h"

#define DEFAULT_FADE_TIME (0.3f)

enum FadeType
{
	FADE_IN,
	FADE_IN_BK,
	FADE_IN_WH,
	FADE_OUT,
	FADE_OUT_BK,
	FADE_OUT_WH
};

class FadeScreen : public Object, public Singleton<FadeScreen>
{
public:
	FadeScreen(void);
	void update(void) override;
	static void Fade(FadeType type, Color color = 0, float interval = DEFAULT_FADE_TIME);
	static bool Finished(void);

private:
	void(FadeScreen::*pUpdate)(void);
	float fadeTime;
	float timer;
	float targetOpacity;
	float oldOpacity;
	Color fadeColor;

	enum AnimeState
	{
		Set,
		Run,
		Stop,
	};

	AnimeState state;

};