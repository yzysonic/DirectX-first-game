#pragma once
#include "Vector.h"
#include "Object.h"
#include "Color.h"
#include <D3d9types.h>

class Camera : public Object
{
public:
	Vector3 at;
	Vector3 up;
	float fov;
	float view_angle;
	float view_aspect;
	float view_near_z;
	float view_far_z;

	Camera(Vector3 pos = Vector3(0, 0, -1.0f));
	~Camera(void);

	D3DXMATRIX getViewMatrix(bool update);
	D3DXMATRIX getProjectionMatrix(bool update);
	void setBackColor(Color color);
	void setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	D3DCOLOR getD3DBackColor(void);
private:
	D3DCOLOR backColor;
	D3DXMATRIX view;
	D3DXMATRIX projection;
};