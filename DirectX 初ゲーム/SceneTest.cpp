#include "SceneTest.h"
#include "Time.h"
#include "Renderer.h"
#include "Camera.h"
#include "test.h"
#include "Player.h"

#define testMax 2000
#define ProjectMode 0

typedef struct _SceneTest
{
	Scene base;
	Camera* camera = NULL;
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
	g_SceneTest.camera = NewObj(Camera);
	g_SceneTest.player = NewObj(Player);

	g_SceneTest.camera->target = g_SceneTest.player->base->transform;
	g_SceneTest.testCount = 0;

	Renderer_SetCamera(g_SceneTest.camera->base->transform);

	for (int j = 0; j < 1000; j++)
	{
		Test *test = NewObj(Test);
		g_SceneTest.testList[g_SceneTest.testCount++] = test;
	}

	Renderer_SetFov(ProjectMode);
}

void updateSceneTest(void)
{
	static float timer = 0;
	static int &i = g_SceneTest.testCount;
	static bool bReset = false;
	static float fov = Renderer_GetFov();
	static int project = ProjectMode;

	timer += GetDeltaTime();

	if (i< testMax && !bReset)
	{
		for (int j = 0; j < 3; j++)
		{
			Test *test = NewObj(Test);
			g_SceneTest.testList[i++] = test;
			if (i >= testMax)
				break;
		}
	}
	Renderer_SetFov(Lerpf(Renderer_GetFov(), (float)project, GetDeltaTime()*5));

	if (GetKeyboardPress(DIK_RETURN))
	{
		bReset = true;
	}

	if (GetKeyboardTrigger(DIK_TAB))
	{
		project = !project;
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

	int line = 0;
	sprintf(GetDebugText(line++), "ObjectCount: %d", i);
	sprintf(GetDebugText(line++), "DeltaTime: %3.0fms", GetDeltaTime()*1000);
	sprintf(GetDebugText(line++), "fov: %2.1f", Renderer_GetFov());
	sprintf(GetDebugText(line++), "PlayerX: %5.1f", g_SceneTest.player->base->transform->position.x);
	sprintf(GetDebugText(line++), "PlayerY: %5.1f", g_SceneTest.player->base->transform->position.y);
	sprintf(GetDebugText(line++), "PlayerZ: %5.1f", g_SceneTest.player->base->transform->position.z);
	sprintf(GetDebugText(line++), "CameraX: %5.1f", g_SceneTest.camera->base->transform->position.x);
	sprintf(GetDebugText(line++), "CameraY: %5.1f", g_SceneTest.camera->base->transform->position.y);
	sprintf(GetDebugText(line++), "CameraZ: %5.1f", g_SceneTest.camera->base->transform->position.z);

}

void uninitSceneTest(void)
{
	DeleteObj(g_SceneTest.camera);
	DeleteObj(g_SceneTest.player);

	int &i = g_SceneTest.testCount;
	while (i > 0)
	{
		i--;
		DeleteObj(g_SceneTest.testList[i]);
	}
}