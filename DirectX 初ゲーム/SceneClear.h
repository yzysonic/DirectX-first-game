#pragma once
#include "Scene.h"
#include "NumberUI.h"

class SceneClear : public Scene, public Singleton<SceneClear>
{
public:
	void init(void) override;
	void update(void) override;
	void uninit(void) override;

private:
	Object *bk;
	NumberUI *score;
	float timer;
	void(SceneClear::*pUpdate)(void);

	void update_clear_fadeWait(void);
	void update_clear_state0(void);
	void update_clear_state1(void);
};