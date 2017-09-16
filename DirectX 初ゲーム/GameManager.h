#pragma once
#include "Scene.h"

#ifdef _DEBUG
#define START_SCENE SceneName::TITLE
#else
#define START_SCENE SceneName::TITLE
#endif

class GameManager : public Singleton<GameManager>
{
public:
	static void Create(void);
	static void Destroy(void);
	static void Update(void);
	static void SetScene(SceneName scene);

private:
	Scene* currentScene;
	static void LoadScene(SceneName scene);
};
