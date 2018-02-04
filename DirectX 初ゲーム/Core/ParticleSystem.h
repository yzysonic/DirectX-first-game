#pragma once

#include "Core.h"
#include <functional>
#include <unordered_map>

struct VtxParticleGeometry
{
	Vector3 pos;
};

struct VtxParticleInstance
{
	D3DXMATRIX	mtx_world;
	Color		color;
};

struct ParticleElement
{
	Transform transform;
	Color color;
	FrameTimer timer;
	Vector3 dir;
	bool active = false;
};

class IParticleBehavior
{
public:
	virtual void Init(ParticleElement& element) = 0;
	virtual void Update(ParticleElement& element) = 0;
};

class ParticleDefaultBehavior : public IParticleBehavior
{
public:
	float lifetime;
	float damping;
	float start_speed;
	float start_size;
	float start_opacity;
	Color start_color;

	float end_size;
	float end_opacity;
	Color end_color;

	ParticleDefaultBehavior(void);
	void Init(ParticleElement& element) override;
	void Update(ParticleElement& element) override;
	void SetTimingFunc(std::string attr, std::function<float(float, float, float)> func);

private:
	std::unordered_map<std::string, std::function<float(float, float, float)>> timing_func;
};

class ParticleSystem : public Drawable, public ILostAndReset
{
public:
	ParticleSystem(UINT particle_max = 100);
	~ParticleSystem(void);

	void Draw(void) override;
	HRESULT OnLostDevice(void) override;
	HRESULT OnResetDevice(void) override;

	void SetMaxNum(UINT value);
	void SetBehavior(IParticleBehavior* behavior);
	void SetDuration(float value);
	UINT GetParticleNum(void);
	template<class T>
	T* GetBehavior(void);

	float emission_rate;
	bool loop;

private:
	static ParticleDefaultBehavior default_behavior;

private:
	IDirect3DVertexDeclaration9 *decl;
	LPDIRECT3DINDEXBUFFER9 pIndexBuff;
	LPDIRECT3DVERTEXBUFFER9 pGeometryBuff;
	LPDIRECT3DVERTEXBUFFER9 pInstanceBuff;

	ParticleElement* elements;
	IParticleBehavior* behavior;
	FrameTimer timer_duration;
	FrameTimer timer_emission;
	UINT particle_max;
	UINT particle_num;
	float duration;

	void Update(void);
	void InitDraw(void);
	void UninitDraw(void);
};

template<class T>
inline T * ParticleSystem::GetBehavior(void)
{
	static_assert(std::is_base_of<IParticleBehavior, T>::value, "Not a subclass of IParticleBehavior.");
	return dynamic_cast<T*>(this->behavior);
}
