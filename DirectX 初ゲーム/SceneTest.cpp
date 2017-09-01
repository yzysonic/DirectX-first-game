#include "SceneTest.h"
#include "test.h"
#include "Time.h"
#include "Renderer.h"

#define testMax 4000

typedef struct _SceneTest
{
	Scene base;
	Player* player = NULL;
	Test* testList[testMax];
	int testCount;
}SceneTest;

SceneTest g_SceneTest;

Scene * GetSceneTest(void)
{
	return (Scene*)&g_SceneTest;
}

void initSceneTest(void)
{
	SetNewObj(g_SceneTest.player, Player);
	g_SceneTest.testCount = 0;
}

void updateSceneTest(void)
{
	static float timer = 0;
	static int &i = g_SceneTest.testCount;
	static bool bReset = false;

	timer += GetDeltaTime();

	if (i< testMax && !bReset)
	{
		for (int j = 0; j < 3; j++)
		{
			Test *test;
			SetNewObj(test, Test);
			g_SceneTest.testList[i++] = test;
			if (i >= testMax)
				break;
		}
		timer = 0;
	}

	if (GetKeyboardPress(DIK_RETURN))
	{
		bReset = true;
	}

	if (bReset)
	{
		for (int j = 0; j < 100; j++)
		{
			if (i > 0)
			{
				i--;
				DeleteObj(g_SceneTest.testList[i]);
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

void uninitSceneTest(void)
{
	DeleteObj(g_SceneTest.player);
	int &i = g_SceneTest.testCount;
	while (i > 0)
	{
		i--;
		DeleteObj(g_SceneTest.testList[i]);
	}
}