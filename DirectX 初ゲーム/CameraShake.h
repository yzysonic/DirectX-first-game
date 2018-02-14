#pragma once

#include "Core\Core.h"

class CameraShake : public Script
{
public:
	float strength;		// 振動の強度（大きいほど幅が広くなる）
	float speed;		// 振動の速さ（大きいほど頻繁になる）
	float decay;		// 振動の減衰（大きいほど減衰が早い）
	float duration;		// 時間（フレーム単位）

	CameraShake(void);
	void Init(void) override;
	void Update(void) override;
	void Shake(float strength = 20.0f);

private:
	Camera* camera;
	int shake_pattern;		// 振動のパターン
	float shake_timer;
};