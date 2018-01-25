#pragma once
#include "Core/Scene.h"
#include "Core\Core.h"

class SceneGlobal : public Scene
{
public:
	int control_type;

public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
};