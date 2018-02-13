#include "EaseMove.h"

EaseMove::EaseMove(void)
{
	active_on_load = false;
}

void EaseMove::Update(void)
{
	float progress = min(timer.Progress(), 1.0f);

	object->transform.position.x = Ease::ExpoInOut(last_pos.x, target_pos.x, progress);
	object->transform.position.y = Ease::ExpoInOut(last_pos.y, target_pos.y, progress);
	object->transform.position.z = Ease::ExpoInOut(last_pos.z, target_pos.z, progress);

	timer.Step();

	if (progress >= 1.0f)
		SetActive(false);
}

void EaseMove::SetInterval(float value)
{
	interval = value;
}

void EaseMove::MoveTo(Vector3 pos)
{
	last_pos = object->transform.position;
	target_pos = pos;
	timer.Reset(interval);
	SetActive(true);
}

void EaseMove::MoveTo(float x, float y, float z)
{
	MoveTo(Vector3(x, y, z));
}
