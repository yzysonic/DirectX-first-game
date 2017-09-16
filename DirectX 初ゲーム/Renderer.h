#pragma once

#include "main.h"
#include "Polygon.h"
#include "Object.h"
#include "Layer.h"
#include "Singleton.h"
#include "Camera.h"
#include <vector>

// レイヤーごとのプールサイズ
const int g_PoolSize[(int)Layer::MAX] =
{
	10,			// BG00
	10,			// BG01
	10,			// BG02
	ObjectMax,	// DEFAULT
	10,			// PLAYER
	 1,			// MASK
	30,			// UI00
	10,			// UI01
	10,			// UI02
	10,			// TOP
};

class Renderer : public Singleton<Renderer>
{
public:
	Renderer(void) {};
	static void Create(void);
	static void Destroy(void);
	void drawFrame(void);
	void addList(RectPolygon* poly);
	void removeList(RectPolygon* poly);
	Camera* getCamera(void);
	void setCamera(Camera* camera);

private:
	std::vector<RectPolygon*> list[(int)Layer::MAX];
	Camera fixedCamera;
	Camera* camera;

	void transformVertex(RectPolygon* poly);
};

char *GetDebugText(int line);
