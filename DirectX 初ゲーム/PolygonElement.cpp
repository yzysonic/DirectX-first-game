#include "PolygonElement.h"

void update_polyele_state0(PolygonElement* thiz);
void update_polyele_state1(PolygonElement* thiz);

void initPolygonElement(Object * thiz)
{
	SetThis(PolygonElement);
	thiz->polygon = newPolygon(thiz);
	thiz->transform->scale = Vector3(0.1f, 0.1f, 1.0f);

	thiz->transform->position.x = Randomf(-SCREEN_CENTER_X - 100, SCREEN_CENTER_X + 100);
	thiz->transform->position.y = Randomf(-SCREEN_CENTER_Y - 100, SCREEN_CENTER_Y + 100);
	thiz->transform->position.z = Randomf(0.01f, 5);

	thizz->timer = 0;
	thizz->timerInterval = Randomf(0.3f, 1.0f);
	thizz->speed = Vector3(Randomf(-1, 1), Randomf(-1, 1), 0.0f)*30.0f;
	thizz->targetOpacity = 1.0f;

	Polygon_SetColor(thiz->polygon, ColorRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 0));

	thizz->update = &update_polyele_state0;
}

void updatePolygonElement(Object * thiz)
{
	SetThis(PolygonElement);
	thizz->update(thizz);
}

void uninitPolygonElement(Object * thiz)
{
}

void update_polyele_state0(PolygonElement* thiz)
{
	float interval = 0.2f;
	if (thiz->timer < interval)
	{
		Polygon_SetOpacity(thiz->base->polygon, thiz->targetOpacity*(thiz->timer / interval));
		thiz->base->transform->scale.x = Lerpf(0.1f, 0.4f, (thiz->timer / interval));
		thiz->base->transform->scale.y = Lerpf(0.1f, 0.4f, (thiz->timer / interval));
		thiz->timer += GetDeltaTime();
	}
	else
	{
		thiz->update = &update_polyele_state1;
	}
}

void update_polyele_state1(PolygonElement* thiz)
{
	if (thiz->timer >= thiz->timerInterval)
	{
		thiz->speed = Vector3(Randomf(-1, 1), Randomf(-1, 1), 0.0f)*30.0f;
		thiz->timerInterval = Randomf(0.2f, 0.7f);
		thiz->timer = 0;
	}

	thiz->base->transform->position += thiz->speed*GetDeltaTime();

	thiz->timer += GetDeltaTime();
}
