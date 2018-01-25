#pragma once
#include "Core\Core.h"
#include "NumberUI.h"

class ScoreUI : public NumberUI
{
public:
	static constexpr float UpdateTime = 0.5f/300.0f;
public:
	ScoreUI(int digits, int x, int y, std::string texture_digit, std::string texture_title);
	void Update(void) override;
	void SetScore(unsigned int value);

private:
	int score;
	int diff;
	float timer;
};