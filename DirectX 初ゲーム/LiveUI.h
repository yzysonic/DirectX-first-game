#pragma once
#include "Core\Core.h"

class LiveUI : public Object
{
public:
	static constexpr float AnimeTime = 0.3f;
public:
	LiveUI(void);
	~LiveUI(void);

	void Update(void) override;
	void SetLowLive(bool value);

private:
	float timer;
	bool low_live;
};