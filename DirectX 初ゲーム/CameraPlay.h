#pragma once
#include "Core\Core.h"

class CameraPlay : public Script
{
public:
	CameraPlay(void);
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	Camera* camera;
	Transform last_transform;
	Object* cursor;
	float phi;				// ’‹“_‚Æ‚ÌƒÓ‰ñ“]ŠpiŒ»İj
	float theta;			// ’‹“_‚Æ‚ÌƒÆ‰ñ“]ŠpiŒ»İj
	float move_phi;			// ƒÓ‰ñ“]Šp‚ÌˆÚ“®—Ê
	float move_theta;		// ƒÆ‰ñ“]Šp‚ÌˆÚ“®—Ê
	float target_dis;		// –Ú•W‹——£
	float dis;				// ’‹“_‚Æ‚Ì‹——£

	void MoveCamera(void);
};
