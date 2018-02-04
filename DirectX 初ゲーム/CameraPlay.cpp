#include "CameraPlay.h"


CameraPlay::CameraPlay(void)
{
	active_on_load = false;
}

void CameraPlay::Init(void)
{
	camera = dynamic_cast<Camera*>(object);

	theta = atan2f(sqrtf(camera->transform.position.x*camera->transform.position.x + camera->transform.position.z*camera->transform.position.z), camera->transform.position.y);
	phi = atan2f(camera->transform.position.z, camera->transform.position.x);
	move_theta = 0.0f;
	move_phi = 0.0f;
	dis = camera->transform.position.length();
	target_dis = dis;

}

void CameraPlay::Update(void)
{
	MoveCamera();

	// 座標変換
	camera->transform.position.y = dis * cosf(theta);
	camera->transform.position.x = dis * sinf(theta) * cosf(phi);
	camera->transform.position.z = dis * sinf(theta) * sinf(phi);
	camera->transform.position += camera->at;

}


void CameraPlay::MoveCamera(void)
{
	Vector3 move = Vector3(0.0f, 0.0f, 0.0f);

	// マウス操作
	if (IsMouseCenterPressed())
	{
		// 移動
		if (GetKeyboardPress(DIK_LSHIFT))
		{
			move.x = -GetMouseMoveX() / 5.0f;
			move.y = GetMouseMoveY() / 5.0f;
		}
		// 回転
		else
		{
			move_phi = -GetMouseMoveX() / 500.0f;
			move_theta = -GetMouseMoveY() / 500.0f;
		}
	}

	// ズーム
	target_dis += -GetMouseMoveZ() / 12.0f;
	dis = dis + (target_dis - dis)*0.15f;


	// キーボード操作
	// 移動
	if (GetKeyboardPress(DIK_LSHIFT))
	{
		if (GetKeyboardPress(DIK_LEFT))
			move.x = -2.0f;
		if (GetKeyboardPress(DIK_RIGHT))
			move.x = 2.0f;
		if (GetKeyboardPress(DIK_UP))
			move.y = 2.0f;
		if (GetKeyboardPress(DIK_DOWN))
			move.y = -2.0f;
	}
	// 回転
	else
	{
		if (GetKeyboardPress(DIK_LEFT))
			move_phi = 0.02f;
		if (GetKeyboardPress(DIK_RIGHT))
			move_phi = -0.02f;
		if (GetKeyboardPress(DIK_UP))
			move_theta = 0.005f;
		if (GetKeyboardPress(DIK_DOWN))
			move_theta = -0.005f;
	}


	if (move_phi != 0.0f)
	{
		phi += move_phi;
		if (fabsf(move_phi) >= 0.001f)
			move_phi *= 0.92f;
		else
			move_phi = 0.0f;
	}
	if (move_theta != 0.0f)
	{
		theta += move_theta;
		if (fabsf(move_theta) >= 0.001f)
			move_theta *= 0.92f;
		else
			move_theta = 0.0f;
	}


	if (move.x != 0.0f || move.y != 0.0f)
	{

		float x = -move.x*cosf(-phi + 0.5f*PI) - move.z*sinf(-phi + 0.5f*PI);
		move.z = move.x*sinf(-phi + 0.5f*PI) - move.z*cosf(-phi + 0.5f*PI);
		move.x = x;

		camera->at += move;
	}


}
