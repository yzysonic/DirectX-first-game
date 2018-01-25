#include "CameraShake.h"

CameraShake::CameraShake()
{
	this->duration = 0.8f;
	this->strength = 20.f;
	this->speed = 70.0f;
	this->decay = 7.0f;
	this->active_on_load = false;
}

void CameraShake::Init(void)
{
	camera = dynamic_cast<Camera*>(object);
	this->shake_pattern = Random(0, 30);
	this->shake_timer = 0.0f;
}

void CameraShake::Update(void)
{
	// �U���I�t�Z�b�g�̌v�Z
	if (shake_timer < duration)
	{
		float progress = shake_timer / duration;
		Vector3 shake_offset;

		// �p�[�����m�C�Y�ŋ[���U��
		shake_offset.x = PerlinNoise(speed * (progress)+shake_pattern);
		shake_offset.y = PerlinNoise(speed * (progress)+shake_pattern + 1);

		// ���x�����̌v�Z
		shake_offset *= exp2f(-decay*progress) * strength;

		camera->transform.position += shake_offset;
		camera->at += shake_offset;

		shake_timer += Time::DeltaTime();
	}

	// �U���̏I���
	else
	{
		//shake_offset = Vector3::zero;
		this->SetActive(false);
	}

}
