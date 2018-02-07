#pragma once
#include "Core\Core.h"

class FieldPolygon : public Object, public IParticleBehavior
{
public:
	void Init(ParticleElement & element) override;
	void Update(ParticleElement & element) override;

	FieldPolygon(int max);
};