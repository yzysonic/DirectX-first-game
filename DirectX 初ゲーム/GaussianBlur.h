#pragma once
#include "Core\Core.h"

class GaussianBlur : public RectPolygon2D
{
	static constexpr int WeightSize = 10;
public:
	float strength;

	GaussianBlur(Texture* tex_source, RenderTarget* target);
	void Draw(void) override;
	bool SetActive(bool value) override;

private:
	RenderTarget buffer;
	RenderTarget* target;
	float weight[WeightSize];

	void CalculateWeight(void);
};