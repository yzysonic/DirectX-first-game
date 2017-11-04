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
	static void SetGlobalScene(Scene* scene);
	static void SetScene(Scene* scene);

private:
	std::unique_ptr<Scene> scene[2];

	static void SetScene(Scene* scene, int no);
};

template<typename T>
inline T & GameManager::var(std::string key)
{
	static std::unordered_map<std::string, T> map{};
	return map[key];
}
