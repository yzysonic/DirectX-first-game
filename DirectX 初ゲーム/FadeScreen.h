#pragma once
#include "Core\Core.h"
#include "Core\Singleton.h"

enum FadeType
{
	FADE_IN,
	FADE_OUT,
};

class FadeScreen : public Object2D, public Singleton<FadeScreen>
{
public:
	static constexpr float DefaultFadeTime = 0.3f;

public:
	FadeScreen(void);
	void update(void) override;
	static void Fade(FadeType type, Color color = Color::none, float interval = DefaultFadeTime);
	static void FadeIn(Color color, float interval = DefaultFadeTime);
	static void FadeOut(Color color, float interval = DefaultFadeTime);
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