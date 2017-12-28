#pragma once
#include "Core\Core.h"
#include "NumberUI.h"

class ScoreUI : public NumberUI
{
public:
	static constexpr float UpdateTime = 0.5f/300.0f;
public:
	ScoreUI(int digits, int x, int y, Texture* texture_digit, Texture* texture_title);
	void update(void) override;
	void SetScore(unsigned int value);

private:
	int score;
	int diff;
	float timer;
};