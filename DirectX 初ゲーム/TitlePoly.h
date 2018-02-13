#pragma once
#include "Core\Core.h"
#include "FieldPolygon.h"

class TitlePoly : public Object, public IParticleBehavior
{
	static constexpr float PolySize = 20.0f;
	static constexpr float Speed1 = 1.0f;
	static constexpr float Speed2 = 10.0f;
public:
	bool mix_mode;

	TitlePoly(UINT max);
	void SetMixMode(float target_mix_factor);
	float GetMixFactor(void);
	void SetBehavior2(void);

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

	class Behavior2 : public IParticleBehavior
	{
	public:
		void Init(ParticleElement & element) override {};
		void Update(ParticleElement & element) override;
	};

	Behavior2 behavior2;
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