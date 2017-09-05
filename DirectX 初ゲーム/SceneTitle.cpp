#include "SceneTitle.h"
#include "Core.h"
#include "GameManager.h"
#include "PolygonElement.h"

#define TitlePolyMax (300)

enum KeyState
{
	FadeIn,
	FadeOut,
	Wait
};

typedef struct _SceneTitle
{
	Scene base;
	Object *logo;
	Object *presskey;
	Object *info;
	PolygonElement* polyList[TitlePolyMax];
	int polyCount;
	float timer;
	KeyState keyState;
	void(*update)(void);
}SceneTitle;


void update_title_state0(void);
void update_title_state1(void);
void update_title_state2(void);

SceneTitle g_SceneTitle;
SceneTitle* thiz;



Scene * GetSceneTitle(void)
{
	return (Scene*)&g_SceneTitle;
}

void initSceneTitle(void)
{
	thiz = &g_SceneTitle;

	thiz->polyCount = 0;
	thiz->timer = 0;
	thiz->update = &update_title_state0;

	SetVolume(BGM_TITLE, -1000);
	PlayBGM(BGM_TITLE);
}

void updateSceneTitle(void)
{
	g_SceneTitle.update();
}

void uninitSceneTitle(void)
{
	deleteObject(thiz->logo);
	deleteObject(thiz->presskey);
	deleteObject(thiz->info);

	for(int i=0;i<TitlePolyMax;i++)
		DeleteObj(g_SceneTitle.polyList[i]);;

	StopSound(BGM_TITLE);
}

void update_title_state0(void)
{
	if (GetKeyboardAnyKeyTrigger())
	{
		for (int i = 0; thiz->polyCount<TitlePolyMax; i++)
		{
			PolygonElement *poly = NewObj(PolygonElement);
			thiz->polyList[thiz->polyCount++] = poly;
		}

	}
	if (thiz->polyCount < TitlePolyMax)
	{
		for (int i = 0; i < 3; i++)
		{
			PolygonElement *poly = NewObj(PolygonElement);
			thiz->polyList[thiz->polyCount++] = poly;
			if (thiz->polyCount == TitlePolyMax)
				break;
		}
	}
	else 
	{
		thiz->logo = newObject(Obj_Normal);
		thiz->logo->polygon = newPolygon(thiz->logo, LAYER_UI_00, TEX_TITLE_LOGO);
		thiz->logo->transform->position = Vector3(0, 200 - SCREEN_CENTER_Y, 0);
		Polygon_SetOpacity(thiz->logo->polygon, 0);

		thiz->update = &update_title_state1;
	}
}

void update_title_state1(void)
{
	float interval = 0.3f;
	if (thiz->timer < interval)
	{
		float progress = thiz->timer / interval;
		thiz->logo->transform->scale.x = Lerpf(3, 1, progress);
		thiz->logo->transform->scale.y = Lerpf(3, 1, progress);
		Polygon_SetOpacity(thiz->logo->polygon, progress);
		thiz->timer += GetDeltaTime();
	}
	else
	{
		thiz->presskey = newObject(Obj_Normal);
		thiz->presskey->polygon = newPolygon(thiz->presskey, LAYER_UI_00, TEX_TITLE_PRESSKEY);
		thiz->presskey->transform->position = Vector3(0, 550 - SCREEN_CENTER_Y, 0);
		Polygon_SetOpacity(thiz->presskey->polygon, 0);

		thiz->info = newObject(Obj_Normal);
		thiz->info->polygon = newPolygon(thiz->info, LAYER_UI_00, TEX_TITLE_INFO);
		thiz->info->transform->position.x = SCREEN_CENTER_X - thiz->info->polygon->pTexture->size.x / 2 - 30;
		thiz->info->transform->position.y = SCREEN_CENTER_Y - thiz->info->polygon->pTexture->size.y - 10;
		thiz->info->transform->position.z = 0;

		thiz->keyState = FadeIn;
		thiz->timer = 0;
		thiz->update = &update_title_state2;
	}

}

void update_title_state2(void)
{

	if (GetKeyboardAnyKeyTrigger())
	{
		SetScene(SCENE_TEST);
		return;
	}
		

	switch (thiz->keyState)
	{
	case Wait:
	{
		float interval = 0.7f;
		if (thiz->timer > interval)
		{
			thiz->keyState = FadeOut;
			thiz->timer = 0;
		}
		break;
	}
	case FadeOut:
	{
		float interval = 0.3f;
		if (thiz->timer > interval)
		{
			thiz->keyState = FadeIn;
			thiz->timer = 0;
		}
		else
		{
			Polygon_SetOpacity(thiz->presskey->polygon, 1 - thiz->timer / interval);
		}
		break;
	}
	case FadeIn:
		float interval = 0.3f;
		if (thiz->timer > interval)
		{
			thiz->keyState = Wait;
			thiz->timer = 0;
		}
		else
		{
			Polygon_SetOpacity(thiz->presskey->polygon, thiz->timer / interval);
		}
		break;
	}

	thiz->timer += GetDeltaTime();
}
