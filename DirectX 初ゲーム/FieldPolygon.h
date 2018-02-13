#pragma once
#include "Core\Core.h"

class FieldPolygon : public Object, public IParticleBehavior
{
public:
	Vector2 field_size;
	float scale;

	void Init(ParticleElement & element) override;
	void Update(ParticleElement & element) override;

	FieldPolygon(int max);
	FieldPolygon(int max, float size_x, float size_y, float scale, float emission_rate, float opacity);
	bool IsCompelete(void);

private:
	float opacity;
};