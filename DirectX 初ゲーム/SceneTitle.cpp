#include "SceneTitle.h"
#include "Core.h"
#include "GameManager.h"
#include "PolygonElement.h"

#define TitlePolyMax (300)

enum animeState
{
	Hit,
	Phase1,
	Phase2,
	Wait
};

typedef struct _SceneTitle
{
	Scene base;
	Object *vignetting;
	Object *logo;
	Object *presskey;
	Object *info;
	PolygonElement* polyList[TitlePolyMax];
	int polyCount;
	float polyTimer;
	float logoTimer;
	float keyTimer;
	Timer *beat;
	int beatCount;
	animeState polyState;
	animeState logoState;
	animeState keyState;
	void(*update)(void);
}SceneTitle;


void update_title_state0(void);
void update_title_state1(void);
void update_title_state2(void);
void update_title_poly(void);
void update_title_logo(void);
void update_title_presskey(void);

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
	thiz->beat = newTimer(60 / 65.0f);
	thiz->beatCount = 0;
	thiz->update = &update_title_state0;

	thiz->vignetting = newObject();
	thiz->vignetting->polygon = newPolygon(thiz->vignetting, LAYER_TOP, TEX_VIGNETTING);

	SetVolume(BGM_TITLE, -1000);
	PlayBGM(BGM_TITLE);
}

void updateSceneTitle(void)
{
	if (Timer_ElapsedTime(thiz->beat) >= (60/65.0f)*(thiz->beatCount + 1))
	{
		//if(thiz->beatCount % 2)
		//	thiz->logoState = Hit;
		thiz->keyState = Hit;
		if((thiz->beatCount % 4 ) == 0)
			thiz->polyState = Hit;
		thiz->beatCount++;
	}
	g_SceneTitle.update();
}

void uninitSceneTitle(void)
{
	deleteObject(thiz->logo);
	deleteObject(thiz->presskey);
	deleteObject(thiz->info);
	deleteObject(thiz->vignetting);

	for(int i=0;i<TitlePolyMax;i++)
		DeleteObj(g_SceneTitle.polyList[i]);;

	StopSound(BGM_TITLE);
}

void update_title_state0(void)
{
	if (GetKeyboardTrigger(DIK_RETURN))
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
		thiz->logoTimer = 0;
		Polygon_SetOpacity(thiz->logo->polygon, 0);

		thiz->update = &update_title_state1;
	}
}

void update_title_state1(void)
{
	float interval = 0.3f;
	if (thiz->logoTimer <= interval)
	{
		float progress = thiz->logoTimer / interval;
		thiz->logo->transform->scale.x = Lerpf(3, 1, progress);
		thiz->logo->transform->scale.y = Lerpf(3, 1, progress);
		Polygon_SetOpacity(thiz->logo->polygon, progress);
		thiz->logoTimer += GetDeltaTime();
	}
	else
	{
		thiz->logo->transform->scale = Vector3(1.0f, 1.0f, 1.0f);

		thiz->presskey = newObject(Obj_Normal);
		thiz->presskey->polygon = newPolygon(thiz->presskey, LAYER_UI_00, TEX_TITLE_PRESSKEY);
		thiz->presskey->transform->position = Vector3(0, 550 - SCREEN_CENTER_Y, 0);

		thiz->info = newObject(Obj_Normal);
		thiz->info->polygon = newPolygon(thiz->info, LAYER_UI_00, TEX_TITLE_INFO);
		thiz->info->transform->position.x = SCREEN_CENTER_X - thiz->info->polygon->pTexture->size.x / 2 - 30;
		thiz->info->transform->position.y = SCREEN_CENTER_Y - thiz->info->polygon->pTexture->size.y - 10;
		thiz->info->transform->position.z = 0;

		thiz->logoState = Phase2;
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
	update_title_poly();
	update_title_logo();
	update_title_presskey();
}

void update_title_poly(void)
{
	switch (thiz->polyState)
	{
	case Hit:
		for (int i = 0; i < TitlePolyMax; i++)
		{
			thiz->polyList[i]->prePos = thiz->polyList[i]->base->transform->position;
			thiz->polyList[i]->nextColor = ColorRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 200);
		}
		thiz->polyState = Phase1;
		thiz->polyTimer = 0;
		break;
	case Phase1:
	{
		float interval = 0.7f;
		if (thiz->polyTimer <= interval)
		{
			float progress = thiz->polyTimer / interval;
			for (int i = 0; i < TitlePolyMax; i++)
			{
				//Vector3 dir;
				//D3DXVec3Normalize(&dir, &thiz->polyList[i]->prePos);
				//thiz->polyList[i]->base->transform->position += dir*10.0f*GetDeltaTime() / interval;

				//thiz->polyList[i]->base->transform->scale = Vector3(1, 1, 0)*Lerpf(0.4f, 0.6f, progress);

				Polygon_SetColor(thiz->polyList[i]->base->polygon, LerpC(thiz->polyList[i]->base->polygon->color, thiz->polyList[i]->nextColor, progress));
			}
		}
		else
		{
			thiz->polyState = Wait;
			//thiz->polyState = Phase2;
		}
		thiz->polyTimer += GetDeltaTime();
		break;
	}
	case Phase2:

		for (int i = 0; i < TitlePolyMax; i++)
		{
			//thiz->polyList[i]->base->transform->position = LerpV3(thiz->polyList[i]->base->transform->position, thiz->polyList[i]->prePos, GetDeltaTime()*10.0f);

			thiz->polyList[i]->base->transform->scale = Vector3(1, 1, 0)*Lerpf(thiz->polyList[i]->base->transform->scale.x, 0.4f, GetDeltaTime()*3.0f);
		}


		break;
	}

}

void update_title_logo(void)
{
	switch (thiz->logoState)
	{
	case Hit:
		thiz->logoState = Phase1;
		thiz->logoTimer = 0;
		break;
	case Phase1:
	{
		float interval = 0.05f;
		if (thiz->logoTimer <= interval)
		{
			float porgress = thiz->logoTimer / interval;
			thiz->logo->transform->scale = Vector3(1, 1, 0)*Lerpf(1.0f, 1.15f, porgress);
		}
		else
		{
			thiz->logoState = Phase2;
			thiz->logoTimer = 0;
		}
		thiz->logoTimer += GetDeltaTime();
		break;
	}
	case Phase2:
		thiz->logo->transform->scale = Vector3(1, 1, 0)*Lerpf(thiz->logo->transform->scale.x, 1.0f, GetDeltaTime()*3.0f);
		break;
	}

}

void update_title_presskey(void)
{
	switch (thiz->keyState)
	{
	case Hit:
		thiz->keyState = Phase1;
		thiz->keyTimer = 0;
		break;
	case Phase1:
	{
		float interval = 0.05f;
		if (thiz->keyTimer <= interval)
		{
			float porgress = thiz->keyTimer / interval;
			Polygon_SetOpacity(thiz->presskey->polygon, Lerpf(0.95f, 0.5f, porgress));
		}
		else
		{
			thiz->keyState = Phase2;
			thiz->keyTimer = 0;
		}
		thiz->keyTimer += GetDeltaTime();
		break;
	}
	case Phase2:
		Polygon_SetOpacity(thiz->presskey->polygon, Lerpf(Polygon_GetOpacity(thiz->presskey->polygon), 0.95f, GetDeltaTime()*3.0f));
		break;
	}

}
