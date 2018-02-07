#include "Camera.h"
#include "Renderer.h"
#include "Math.h"

Camera::Camera(RenderTarget* render_target)
{
	if (render_target == nullptr)
		this->render_target = RenderTarget::BackBuffer();
	else this->render_target = render_target;
	this->type = ObjectType::Camera;
	this->transform.position = Vector3(0.0f, 0.0f, -250.0f);
	this->setBackColor(Color(255, 255, 255, 255));
	this->at = Vector3(0.0f, 0.0f, 1.0f);
	this->up = Vector3(0.0f, 1.0f, 0.0f);
	this->aspect = (float)SystemParameters::ResolutionX / (float)SystemParameters::ResolutionY;
	this->near_z = 10.0f;
	this->far_z = 1000.0f;
	this->fov = 2.0f*atan2f(0.5f*SystemParameters::ResolutionY, this->transform.position.length());
}

D3DXMATRIX Camera::getViewMatrix(bool update)
{
	if (update)
	{
		// ビューマトリクスの初期化
		D3DXMatrixIdentity(&view);

		// ビューマトリクスの作成
		D3DXVECTOR3 eye = this->transform.position;
		D3DXVECTOR3 at = this->at;
		D3DXVECTOR3 up = this->up;
		D3DXMatrixLookAtLH(&view, &eye, &at, &up);
	}

	return view;
}

D3DXMATRIX Camera::getProjectionMatrix(bool update)
{
	if (update)
	{
		// プロジェクションマトリクスの初期化
		D3DXMatrixIdentity(&projection);

		// プロジェクションマトリクスの作成
		D3DXMatrixPerspectiveFovLH(&projection, fov, aspect, near_z, far_z);
	}

	return projection;
}

void Camera::setBackColor(Color color)
{
	backColor = ColorRGBA(color.r, color.g, color.b, color.a);
}

void Camera::setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	backColor = ColorRGBA(r, g, b, a);
}
