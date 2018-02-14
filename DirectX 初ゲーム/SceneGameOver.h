#pragma once
#include "Core/Scene.h"
#include "Core/Core.h"

class SceneGameOver : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	Object *text;
	float timer;
	void(SceneGameOver::*pUpdate)(void);

	void update_fadeWait(void);
	void update_state0(void);
	void update_state1(void);
};
