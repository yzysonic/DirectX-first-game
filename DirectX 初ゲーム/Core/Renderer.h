#pragma once

#include "Common.h"
#include "Drawable.h"
#include "Object.h"
#include "Layer.h"
#include "Singleton.h"
#include "Camera.h"
#include <vector>

class Renderer : public Singleton<Renderer>
{
public:
	static void Create(void);
	static void Destroy(void);
	static void DrawFrame(void);
	Camera* getCamera(std::string render_space_name , int no = 0);
	Camera* getCamera(int render_space_no = 0, int no = 0);
	void setCamera(Camera* camera, std::string render_space);
	void setCamera(Camera* camera, int render_space = 0);

private:
	Camera fixedCamera;
};

char *GetDebugText(int line);
