#pragma once

typedef enum
{
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_TEST
}SceneName;

typedef struct _Scene
{
	void(*init)(void);
	void(*update)(void);
	void(*uninit)(void);
}Scene;