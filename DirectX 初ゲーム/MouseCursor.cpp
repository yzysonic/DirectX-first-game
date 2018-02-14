#include "MouseCursor.h"

MouseCursor::MouseCursor(void)
{
	this->transform.scale *= 0.15f;
	AddComponent<RectPolygon2D>("cursor", Layer::TOP)->SetOpacity(0.0f);
	this->state = 0;
}

void MouseCursor::Update(void)
{
	if (IsMouseCenterPressed() || IsMouseLeftPressed())
		this->transform.scale = Vector3::one*0.3f;
	else
		this->transform.scale = Vector3::one*0.15f;
	this->transform.position = GetMousePos();

	switch (this->state)
	{
	// ë“ã@
	case 0:
		if (IsMouseMoved())
		{
			GetComponent<RectPolygon2D>()->SetOpacity(1.0f);
			this->timer.Reset(3.0f);
			this->state = 1;
		}
		break;

	// ï\é¶
	case 1:
		if (IsMouseMoved() || IsMouseLeftPressed())
			this->timer.Reset();
		if (this->timer.TimeUp())
		{
			this->timer.Reset(1.0f);
			this->state = 2;
		}
		this->timer++;
		break;

	// è¡Ç¶ÇÈ
	case 2:
		GetComponent<RectPolygon2D>()->SetOpacity(1.0f-this->timer.Progress());
		if (IsMouseMoved() || IsMouseLeftPressed())
		{
			GetComponent<RectPolygon2D>()->SetOpacity(1.0f);
			this->timer.Reset(3.0f);
			this->state = 1;
			return;
		}
		if (this->timer.TimeUp())
		{
			GetComponent<RectPolygon2D>()->SetOpacity(0.0f);
			this->state = 0;
		}
		this->timer++;
		break;
	}

}
