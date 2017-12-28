#pragma once
#include "Core\Core.h"

class LiveUI : public Object2D
{
public:
	static constexpr float AnimeTime = 0.3f;
public:
	LiveUI(void);
	~LiveUI(void);

	void update(void) override;
	void SetLowLive(bool value);

private:
	float timer;
	bool low_live;
};