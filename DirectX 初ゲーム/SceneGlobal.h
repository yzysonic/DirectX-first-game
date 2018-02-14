#pragma once
#include "Core/Scene.h"
#include "Core\Core.h"
#include "PostEffect.h"
#include "MouseCursor.h"

class SceneGlobal : public Scene
{
public:
	int control_type;

public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	//Object* screen;
	//Camera* camera_screen;
	//RenderTarget* render_target;

	PostEffect* post_effect;
	MouseCursor* cursor;
};