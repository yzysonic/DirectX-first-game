#pragma once

#include "Core\Core.h"

class CameraShake : public Script
{
public:
	float strength;		// �U���̋��x�i�傫���قǕ����L���Ȃ�j
	float speed;		// �U���̑����i�傫���قǕp�ɂɂȂ�j
	float decay;		// �U���̌����i�傫���قǌ����������j
	float duration;		// ���ԁi�t���[���P�ʁj

	CameraShake(void);
	void Init(void) override;
	void Update(void) override;
	void Shake(float strength = 20.0f);

private:
	Camera* camera;
	int shake_pattern;		// �U���̃p�^�[��
	float shake_timer;
};