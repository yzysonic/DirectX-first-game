#pragma once
#include "Vector.h"
#include "Object.h"
#include "Color.h"
#include "RenderTarget.h"
#include <D3d9types.h>

class Camera : public Object
{
public:
	Vector3 at;
	Vector3 up;
	float fov;
	float aspect;
	float near_z;
	float far_z;
	D3DCOLOR backColor;
	RenderTarget* render_target;

	Camera(RenderTarget* render_target = nullptr);
	virtual ~Camera(void) {};

	D3DXMATRIX getViewMatrix(bool update);
	D3DXMATRIX getProjectionMatrix(bool update);
	void setBackColor(Color color);
	void setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
private:
	D3DXMATRIX view;
	D3DXMATRIX projection;
};