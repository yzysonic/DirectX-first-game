#pragma once

#include "Core\Core.h"

class CameraShake : public ObjectBase
{
public:
	float strength;	// 振動の強度（大きいほど幅が広くなる）
	float speed;		// 振動の速さ（大きいほど頻繁になる）
	float decay;		// 振動の減衰（大きいほど減衰が早い）
	float duration;		// 時間（フレーム単位）

	CameraShake(void);
	void update(void) override;
	void SetCamera(Camera* camera);
	void Activate(void);

private:
	Camera* camera;
	int shake_pattern;		// 振動のパターン
	float shake_timer;
};