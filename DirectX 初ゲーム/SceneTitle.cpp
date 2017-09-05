#include "SceneTitle.h"
#include "Core.h"
#include "GameManager.h"

typedef struct _SceneTitle
{
	Scene base;
	Object *logo;
	Object *presskey;
	Object *info;
	float timer;
}SceneTitle;

SceneTitle g_SceneTitle;

Scene * GetSceneTitle(void)
{
	return (Scene*)&g_SceneTitle;
}

void initSceneTitle(void)
{
	SceneTitle* thiz = &g_SceneTitle;

	thiz->logo = newObject(ObjType_Normal);
	thiz->logo->polygon = newPolygon(thiz->logo, LAYER_DEFAULT, TEX_TITLE_LOGO);
	thiz->logo->transform->position = Vector3(0, 200- SCREEN_CENTER_Y, 0);

	thiz->presskey = newObject(ObjType_Normal);
	thiz->presskey->polygon = newPolygon(thiz->presskey, LAYER_DEFAULT, TEX_TITLE_PRESSKEY);
	thiz->presskey->transform->position = Vector3(0, 455- SCREEN_CENTER_Y, 0);

	thiz->info = newObject(ObjType_Normal);
	thiz->info->polygon = newPolygon(thiz->info, LAYER_DEFAULT, TEX_TITLE_INFO);
	thiz->info->transform->position.x = SCREEN_CENTER_X - thiz->info->polygon->pTexture->size.x/2 - 30;
	thiz->info->transform->position.y = SCREEN_CENTER_Y - thiz->info->polygon->pTexture->size.y - 10;
	thiz->info->transform->position.z = 0;

	thiz->timer = 0;

	SetVolume(BGM_TITLE, -1000);
	PlayBGM(BGM_TITLE);
}

void updateSceneTitle(void)
{
	if (GetKeyboardAnyKeyTrigger() && g_SceneTitle.timer >= 1.0f)
		SetScene(SCENE_TEST);

	g_SceneTitle.timer += GetDeltaTime();
}

void uninitSceneTitle(void)
{
	SceneTitle* thiz = &g_SceneTitle;
	deleteObject(thiz->logo);
	deleteObject(thiz->presskey);
	deleteObject(thiz->info);

	StopSound(BGM_TITLE);
}
