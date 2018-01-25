#pragma once

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
	virtual ~Scene(void) {};
};