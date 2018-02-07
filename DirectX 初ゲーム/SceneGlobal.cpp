#include "SceneGlobal.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneGameOver.h"
#include "SceneClear.h"
#include "SceneTest.h"
#include "FadeScreen.h"

void SceneGlobal::Init(void)
{
	FadeScreen::Create();
	/*AddObject*/(FadeScreen::GetInstance());
	FadeScreen::FadeOut(Color::black, 0.0f);
	this->control_type = kKeyboard | kMouse;

	Texture::LoadTexture("vignetting");
	Texture::LoadTexture("title_logo");
	Texture::LoadTexture("title_presskey", "title_presskey_v3.png");
	Texture::LoadTexture("title_info", "title_copyright.png");
	Texture::LoadTexture("title_cursor", "title_cursor_v2.png");
	Texture::LoadTexture("title_start");
	Texture::LoadTexture("title_exit");
	Texture::LoadTexture("number", "number.png", 10);
	Texture::LoadTexture("game_score");
	Texture::LoadTexture("game_time");
	Texture::LoadTexture("game_over");
	Texture::LoadTexture("clear");
	Texture::LoadTexture("lives", "lives.png", 3);
	Texture::LoadTexture("player");
	Texture::LoadTexture("enemy");
	Texture::LoadTexture("bullet");
	Texture::LoadTexture("bullet_e");
	Texture::LoadTexture("guide");
	Texture::LoadTexture("player_mark");
	Texture::LoadTexture("enemy_mark");
	Texture::LoadTexture("pause", "pause.jpg");
	Texture::LoadTexture("title_mask");


	VertexShader::Load("InstancingVS.hlsl");

}

void SceneGlobal::Update(void)
{
#ifdef _DEBUG
	GameManager* gm = GameManager::GetInstance();

	if (GetKeyboardTrigger(DIK_F1))
		gm->SetScene(new SceneTitle);
	if (GetKeyboardTrigger(DIK_F2))
		gm->SetScene(new SceneGame);
	if (GetKeyboardTrigger(DIK_F3))
		gm->SetScene(new SceneGameOver);
	if (GetKeyboardTrigger(DIK_F4))
		gm->SetScene(new SceneClear);
	if (GetKeyboardTrigger(DIK_0))
		gm->SetScene(new SceneTest);
#endif
}

void SceneGlobal::Uninit(void)
{
	FadeScreen::Singleton<FadeScreen>::Destroy();

}