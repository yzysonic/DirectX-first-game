#include "TitleCamera.h"
#include "EaseMove.h"
#include "CameraFov.h"
#include "CameraShake.h"
#include "CameraPlay.h"

TitleCamera::TitleCamera(void)
{
	this->at.z = 0.0f;
	this->near_z = 5.0f;
	this->far_z = 7000.0f;

	AddComponent<EaseMove>()->SetInterval(1.0f);
	AddComponent<CameraFov>();
	AddComponent<CameraShake>();
	AddComponent<CameraPlay>();

	auto fov = GetComponent<CameraFov>();
	fov->SetFov(Deg2Rad(1.0f));
	fov->speed = 10.0f;
	setBackColor(210, 210, 210, 255);

	Renderer::GetInstance()->setCamera(this);
	this->state = Orthogonal;
}

TitleCamera::~TitleCamera(void)
{
	Renderer::GetInstance()->setCamera(nullptr);
}

void TitleCamera::SetState(State state)
{
	switch (this->state)
	{
	case Orthogonal:
		if (state == Normal)
		{
			GetComponent<CameraFov>()->SetFov(Deg2Rad(110.0f), 3.0f);
			this->state = Normal;
		}
		break;

	case Normal:
		if (state == Orthogonal)
		{
			GetComponent<CameraFov>()->SetFov(Deg2Rad(1.0f), 3.0f);
			this->state = Orthogonal;
		}
		else if (state == Far)
		{
			GetComponent<EaseMove>()->MoveTo(0.0f, 0.0f, -6700.0f);
			this->state = Far;
		}
		break;

	case Far:
		if (state == Normal)
		{
			GetComponent<EaseMove>()->MoveTo(0.0f, 0.0f, -250.0f);
			this->state = Normal;
		}
		break;
	}
}

void TitleCamera::SetPlay(bool active)
{
	GetComponent<CameraPlay>()->SetActive(active);
}
