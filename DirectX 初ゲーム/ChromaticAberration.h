#pragma once
#include "Core\Core.h"

class ChromaticAberration : public RectPolygon2D
{
public:
	float strength;
	float time;

	ChromaticAberration(Texture* tex_source, RenderTarget* target);
	void Draw(void) override;

private:
	RenderTarget* target;

	float GetScale(void);
};