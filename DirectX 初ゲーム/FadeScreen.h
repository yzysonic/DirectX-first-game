#pragma once
#include "Core/Core.h"
#include "Core/Singleton.h"

#define DEFAULT_FADE_TIME (0.3f)

enum FadeType
{
	FADE_IN,
	FADE_OUT,
};

class FadeScreen : public Object2D, public Singleton<FadeScreen>
{
public:
	FadeScreen(void);
	void update(void) override;
	static void Fade(FadeType type, Color color = Color::none, float interval = DEFAULT_FADE_TIME);
	static void FadeIn(Color color, float interval = DEFAULT_FADE_TIME);
	static void FadeOut(Color color, float interval = DEFAULT_FADE_TIME);
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