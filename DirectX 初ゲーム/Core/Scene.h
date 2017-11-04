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
	virtual void init(void) {};
	virtual void update(void) = 0;
	virtual void uninit(void) {};
};