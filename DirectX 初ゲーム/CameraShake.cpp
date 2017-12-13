#include "CameraShake.h"

CameraShake::CameraShake()
{
	this->camera = camera;
	this->setActive(false);
	this->duration = 0.8f;
	this->strength = 20.f;
	this->speed = 70.0f;
	this->decay = 7.0f;
}

void CameraShake::update(void)
{
	// 振動オフセットの計算
	if (shake_timer < duration)
	{
		float progress = shake_timer / duration;
		Vector3 shake_offset;

		// パーリンノイズで擬似振動
		shake_offset.x = PerlinNoise(speed * (progress)+shake_pattern);
		shake_offset.y = PerlinNoise(speed * (progress)+shake_pattern + 1);

		// 強度減衰の計算
		shake_offset *= exp2f(-decay*progress) * strength;

		camera->getTransform()->position += shake_offset;
		camera->at += shake_offset;

		shake_timer += Time::DeltaTime();
	}

	// 振動の終わり
	else
	{
		//shake_offset = Vector3::zero;
		this->setActive(false);
	}

}

void CameraShake::SetCamera(Camera * camera)
{
	this->camera = camera;
}

void CameraShake::Activate(void)
{
	this->shake_pattern = Random(0, 30);
	this->shake_timer = 0.0f;
	this->setActive(true);
}
