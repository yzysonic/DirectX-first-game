#include "FieldBoundary.h"

FieldBoundary::FieldBoundary(float width, float height)
{
	this->poly = AddComponent<RectPolygon2D>("field_boundary", Layer::MASK);
	this->poly->rendType = RendererType::Classic2D;
	this->poly->setSize(width, height);
	this->poly->SetOpacity(0.0f);
	this->state = 0;
}

void FieldBoundary::Update(void)
{
	switch (this->state)
	{
	case 1:
		this->poly->SetOpacity(Ease::CircOut(this->poly->GetOpacity(), 1.0f, this->timer.Progress()));
		if (this->timer.Progress() >= 1.0f)
		{
			this->timer.Reset(0.5f);
			this->state = 2;
		}
		this->timer++;
		break;

	case 2:
		this->poly->SetOpacity(Lerpf(1.0f, 0.0f, this->timer.Progress()));
		if (this->timer.Progress() >= 1.0f)
			this->state = 0;
		this->timer++;
		break;
	}
}

void FieldBoundary::Touch(void)
{
	if (this->state != 1)
	{
		this->state = 1;
		this->timer.Reset(0.4f);
	}
}
