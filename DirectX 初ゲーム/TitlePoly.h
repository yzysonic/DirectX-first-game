#pragma once
#include "Core\Core.h"
#include "FieldPolygon.h"

class TitlePoly : public Object, public IParticleBehavior
{
	static constexpr float PolySize = 20.0f;
public:
	bool mix_mode;

	TitlePoly(void);
	void SetMixMode(float target_mix_factor);
	float GetMixFactor(void);

private:
	struct Data
	{
		Vector2 pos;
		Color color;
	};

	struct Element : public ParticleElement
	{
		Vector3 fix_pos;
		Color fix_color1;
		Color fix_color2;
	};

	std::vector<std::vector<Data>> data_map;
	RandomTable random_table;
	Camera* camera = nullptr;
	Vector2 size;
	float scale;
	float mix_factor;
	float target_mix_factor;

	void MakeElement(ParticleElement** elements, UINT* pitch, UINT number) override;
	void Init(ParticleElement & element) override;
	void Update(ParticleElement & element) override;
	void Update(void) override;
};