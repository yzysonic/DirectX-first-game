#pragma once

#include "Scene.h"
#include <memory>

#ifdef _DEBUG
#define START_SCENE SceneName::TEST
#else
#define START_SCENE SceneName::TITLE
#endif

class GameManager : public Singleton<GameManager>
{
public:
	int score;

	static void Create(void);
	static void Destroy(void);
	static void Update(void);
	static void SetScene(SceneName scene);

private:
	std::unique_ptr<Scene> currentScene;
	static void LoadScene(SceneName scene);
};
