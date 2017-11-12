#pragma once
#include "Core/Scene.h"
#include "Core\Core.h"
#include "Core\Singleton.h"

class SceneGlobal : public Scene, public Singleton<SceneGlobal>
{
public:
	int control_type;

public:
	void init(void) override;
	void update(void) override;
	void uninit(void) override;
};