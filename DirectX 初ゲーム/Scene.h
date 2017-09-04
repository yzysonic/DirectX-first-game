#pragma once

enum SceneName
{
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_TEST
};

typedef struct _Scene
{
	void(*init)(void);
	void(*update)(void);
	void(*uninit)(void);
}Scene;