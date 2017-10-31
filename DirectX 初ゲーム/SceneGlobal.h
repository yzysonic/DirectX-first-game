#pragma once
#include "Scene.h"

class SceneGlobal : public Scene
{
public:
	void init(void) override;
	void update(void) override;
	void uninit(void) override;

};