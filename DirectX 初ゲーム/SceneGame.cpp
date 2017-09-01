#include "SceneGame.h"
#include "test.h"
#include "Time.h"
#include "Renderer.h"

#define testMax 10

typedef struct _SceneGame
{
	Scene base;
	Player* player = NULL;
	Test* testList[testMax];
}SceneGame;

SceneGame g_SceneGame;

Scene * GetSceneGame(void)
{
	return (Scene*)&g_SceneGame;
}

void initSceneGame(void)
{
	//g_SceneGame.player = newPlayer();
	SetNewObj(g_SceneGame.player, Player);
}

void updateSceneGame(void)
{
	static float timer = 0;
	static int i = 0;
	static bool bReset = false;

	timer += GetDeltaTime();

	if (i<= testMax && !bReset)
	{
		for (int j = 0; j < 3; j++)
		{
			Test *test;
			SetNewObj(test, Test);
			g_SceneGame.testList[i] = test;
			i++;
		}
		timer = 0;
	}

	if (GetKeyboardPress(DIK_RETURN))
	{
		bReset = true;
	}

	if (bReset)
	{
		for (int j = 0; j < 10; j++)
		{
			if (i >= 0)
			{
				DeleteObj(g_SceneGame.testList[i]);
				i--;
			}
			else
			{
				bReset = false;
				break;
			}
		}
	}

	sprintf(GetDebugText(), "ObjectCount: %d", i);
	
}

void uninitSceneGame(void)
{
	//deletePlayer(g_SceneGame.player);
	DeleteObj(g_SceneGame.player);
}

Player * GetPlayer(void)
{
	return g_SceneGame.player;
}
