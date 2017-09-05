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
	thiz->transform->position.z = Randomf(1, 5);

	thisPolygonElement->timer = 0;
	thisPolygonElement->timerInterval = Randomf(0.2f, 0.7f);
	thisPolygonElement->speed = Vector3(Randomf(-50, 50), Randomf(-50, 50), 0.0f) * 2;

	Polygon_SetColor(thiz->polygon, ColorRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 0));

	thisPolygonElement->update = &update_polyele_state0;
}

void updatePolygonElement(Object * thiz)
{
	SetThis(PolygonElement);
	thisPolygonElement->update(thisPolygonElement);
}

void uninitPolygonElement(Object * thiz)
{
}

void update_polyele_state0(PolygonElement* thiz)
{
	float interval = 0.2f;
	if (thiz->timer < interval)
	{
		Polygon_SetOpacity(thiz->base->polygon, thiz->timer / interval);
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
		thiz->speed = Vector3(Randomf(-50, 50), Randomf(-50, 50), 0.0f) * 2;
		thiz->timerInterval = Randomf(0.2f, 0.7f);
		thiz->timer = 0;
	}

	thiz->base->transform->position += thiz->speed*GetDeltaTime();

	thiz->timer += GetDeltaTime();
}
