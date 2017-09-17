#pragma once

#include"Scene.h"
#include "Core.h"
#include "PolygonElement.h"

// �}�N����`
#define TitlePolyMax (700)
#define TitleBGMBPS (130)

class SceneTitle : public Scene, public Singleton<SceneTitle>
{
public:
	void init(void) override;
	void update(void) override;
	void uninit(void) override;

private:
	// �A�j���[�V������Ԃ̗�
	enum AnimeState
	{
		Hit,
		Phase1,
		Phase2,
		Wait
	};

	enum Command
	{
		COM_START,
		COM_EXIT,
		COM_WAIT,
		COM_OP,
	};

	// �摜
	Object *vignetting = nullptr;
	Object *logo = nullptr;
	Object *presskey = nullptr;
	Object *info = nullptr;
	Object *cursor = nullptr;
	Object *start = nullptr;
	Object *exit = nullptr;

	// �w�i�|���S��
	PolygonElement* polyList[TitlePolyMax];
	int polyCount;

	// ���Ԍv��
	float timer;
	Timer *beat = nullptr;
	int beatCount;

	// �A�j���[�V�������
	AnimeState polyState;
	AnimeState logoState;
	AnimeState keyState;

	// �J�[�\��
	int cursorPos;

	Command com;

	// ��ԃ|�C���^
	void(SceneTitle::*pUpdate)(void);

	void syncAnimation(void);
	void update_fadeWait(void);
	void update_createPoly(void);
	void update_showLogo(void);
	void update_pressWait(void);
	void update_showMenu_A(void);
	void update_showMenu_B(void);
	void update_menu(void);

	void update_anime_poly(void);
	void update_anime_logo(void);
	void update_anime_presskey(void);

};
