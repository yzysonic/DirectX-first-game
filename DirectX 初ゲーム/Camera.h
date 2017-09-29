#pragma once
#include "Vector.h"
#include "Object.h"
#include "Color.h"
#include <D3d9types.h>

class Camera : public Object
{
public:
	float fov;

	Camera(Vector3 pos = Vector3(0, 0, -1.0f));
	~Camera(void);

	void setBackColor(Color color);
	void setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	D3DCOLOR getD3DBackColor(void);
private:
	D3DCOLOR backColor;
};