#pragma once
#include "Core\Core.h"

class TitleCamera : public Camera
{
public:
	enum State
	{
		Orthogonal,
		Normal,
		Far
	};

	TitleCamera(void);
	~TitleCamera(void);
	void SetState(State state);
	void SetPlay(bool active);

private:
	State state;
};