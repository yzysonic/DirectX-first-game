#pragma once
#include "Core\Core.h"
#include "ChromaticAberration.h"

class ChromaticAberrationScript : public Script
{
public:
	float interval;
	bool pause;

	ChromaticAberrationScript(void);
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	FrameTimer timer;
	ChromaticAberration* ca;
	int state;
};