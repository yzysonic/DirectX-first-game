#pragma once
#include "Core\Core.h"

class FieldPolygonBehavior : public IParticleBehavior
{
public:
	void Init(ParticleElement& element);
	void Update(ParticleElement& element);
};

class FieldPolygon : public Object
{
public:
	FieldPolygon(int max);
private:
	static FieldPolygonBehavior behavior;
};