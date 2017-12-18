#include "Camera.h"
#include "Renderer.h"
#include "Math.h"

Camera::Camera(RenderTarget* render_target)
{
	if (render_target == nullptr)
		this->render_target = RenderTarget::BackBuffer();
	else this->render_target = render_target;
	this->type = ObjectType::Camera;
	this->transform->position = Vector3(0.0f, 0.0f, -1.0f);
	this->setBackColor(Color(255, 255, 255, 255));
	this->fov = 0.0f;
	this->at = Vector3(0.0f, 0.0f, 1.0f);
	this->up = Vector3(0.0f, 1.0f, 0.0f);
	this->view_angle = Deg2Rad(150.0f);
	this->view_aspect = (float)SystemParameters::ResolutionX / (float)SystemParameters::ResolutionY;
	this->view_near_z = 0.1f;
	this->view_far_z = 1000.0f;
}

D3DXMATRIX Camera::getViewMatrix(bool update)
{
	if (update)
	{
		// ビューマトリクスの初期化
		D3DXMatrixIdentity(&view);

		// ビューマトリクスの作成
		D3DXVECTOR3 eye = D3DXVECTOR3(transform->position.x, transform->position.y, transform->position.z);
		D3DXVECTOR3 at = D3DXVECTOR3(this->at.x, this->at.y, this->at.z);
		D3DXVECTOR3 up = D3DXVECTOR3(this->up.x, this->up.y, this->up.z);
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
		D3DXMatrixPerspectiveFovLH(&projection, view_angle, view_aspect, view_near_z, view_far_z);
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
