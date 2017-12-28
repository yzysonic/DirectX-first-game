#pragma once

#include "Core\Core.h"

class CameraShake : public ObjectBase
{
public:
	float strength;	// �U���̋��x�i�傫���قǕ����L���Ȃ�j
	float speed;		// �U���̑����i�傫���قǕp�ɂɂȂ�j
	float decay;		// �U���̌����i�傫���قǌ����������j
	float duration;		// ���ԁi�t���[���P�ʁj

	CameraShake(void);
	void update(void) override;
	void SetCamera(Camera* camera);
	void Activate(void);

private:
	Camera* camera;
	int shake_pattern;		// �U���̃p�^�[��
	float shake_timer;
};