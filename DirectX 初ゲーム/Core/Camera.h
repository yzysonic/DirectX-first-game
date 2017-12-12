#pragma once
#include "Vector.h"
#include "Object.h"
#include "Color.h"
#include "RenderTarget.h"
#include <D3d9types.h>

class Camera : public ObjectBase
{
public:
	Vector3 at;
	Vector3 up;
	float fov;
	float view_angle;
	float view_aspect;
	float view_near_z;
	float view_far_z;
	D3DCOLOR backColor;
	RenderTarget* render_target;

	Camera(RenderTarget* render_target = nullptr);
	~Camera(void);

	D3DXMATRIX getViewMatrix(bool update);
	D3DXMATRIX getProjectionMatrix(bool update);
	void setBackColor(Color color);
	void setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
private:
	D3DXMATRIX view;
	D3DXMATRIX projection;
};