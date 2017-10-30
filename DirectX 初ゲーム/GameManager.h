#pragma once

#include "Scene.h"
#include "Singleton.h"
#include <memory>
#include <unordered_map>

#ifdef _DEBUG
#define START_SCENE SceneName::TEST
#else
#define START_SCENE SceneName::TITLE
#endif

class GameManager : public Singleton<GameManager>
{
public:
	template<typename T>
	T& var(std::string key);

	static void Create(void);
	static void Destroy(void);
	static void Update(void);
	static void SetScene(Scene* scene);
	static void LoadScene(Scene* scene);

private:
	std::unique_ptr<Scene> currentScene;
};

template<typename T>
inline T & GameManager::var(std::string key)
{
	static std::unordered_map<std::string, T> map{};
	return map[key];
}
