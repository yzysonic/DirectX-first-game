#pragma once
#include "Core/Scene.h"
#include "Core/Core.h"

class SceneGameOver : public Scene
{
public:
	void init(void) override;
	void update(void) override;
	void uninit(void) override;

private:
	Object *bk;
	float timer;
	void(SceneGameOver::*pUpdate)(void);

	void update_fadeWait(void);
	void update_state0(void);
	void update_state1(void);
};
