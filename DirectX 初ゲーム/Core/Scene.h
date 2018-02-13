#pragma once
#include <vector>
#include "Common.h"
#include "Object.h"

enum struct SceneName
{
	TITLE,
	GUIDE,
	GAME,
	GAMEOVER,
	CLEAR,
	TEST
};

class Scene 
{
public:
	virtual void Init(void) {};
	virtual void Update(void) = 0;
	virtual void Uninit(void) {};
	virtual void OnPause(void) {};
	virtual void OnResume(void) {};
	virtual ~Scene(void);

	template<class T>
	T* AddObject(T* object);
	void RemoveObject(Object* object);
	void UpdateObjects(void);
	void PauseObjects(void);
	void ResumeObjects(void);

private:
	std::vector<Object*> obj_list;
	std::vector<bool> active_shot;
};

template<class T>
inline T * Scene::AddObject(T * object)
{
	static_assert(std::is_base_of<Object, T>::value, "Not a subclass of Object.");

	this->obj_list.emplace_back();
	this->obj_list.back() = object;
	object->scene = this;
	object->sceneIndex = this->obj_list.size() - 1;
	return object;
}