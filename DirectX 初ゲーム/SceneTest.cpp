#include "SceneTest.h"
#include "Time.h"
#include "Core/Renderer.h"
#include "Core\RenderSpace.h"
#include "FadeScreen.h"
#include "CameraSmooth.h"
#include "CameraFov.h"
#include "CameraShake.h"
#include "CameraPlay.h"
#include "EaseMove.h"

#define testMax 2000
#define ProjectMode 0

void SceneTest::Init(void)
{

	this->player = new Player;
	//this->player->SetActive(false);
	//this->enemy = new Enemy;
	//enemy->target = &this->player->transform;
	//enemy->transform.position = Vector3(30, 30, 0);

	Direct3D::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	this->camera = new TitleCamera();


	// イベントバインド
	//this->player->injury += [&]
	//{
	//	this->mini_map->Shake();
	//};
	//if (this->camera->GetComponent<CameraShake>() != nullptr)
	//{
	//	this->player->injury += [&]
	//	{
	//		this->camera->GetComponent<CameraShake>()->SetActive(true);
	//	};
	//}

	//this->mini_map = new MiniMap(200, 200, 11);
	//this->mini_map->SetPosition(Vector3(SystemParameters::ResolutionX / 2.0f - 150.0f, -SystemParameters::ResolutionY / 2.0f + 150.0f, 0.0f));
	//this->mini_map->SetPlayer(this->player);
	//this->mini_map->SetEnemy(this->enemy);



	this->PolyMax = 12000;
	this->test = (new Object);
	auto particle = 
	this->test->AddComponent<ParticleSystem>(1000);
	particle->GetBehavior<ParticleDefaultBehavior>()->start_color = Color(0, 0, 255, 255);
	particle->SetDuration(0.5f);
	particle->emission_rate = 100.0f;

	this->field_bk = new FieldPolygon(PolyMax/6);
	this->polyCount = PolyMax;

	this->title_poly = new TitlePoly(PolyMax*5/6);
	/*this->title_poly->SetActive(false);*/

	this->logo = new Object;
	this->logo->AddComponent<RectPolygon>("title_logo", Layer::UI_00);
	this->logo->transform.position = Vector3(0, SystemParameters::ResolutionY / 2.f - 200, 0);


	SetVolume(BGM_GAME, -1800);
	FadeScreen::FadeIn(Color::black, 0.0f);
	//PlayBGM(BGM_GAME);
}

void SceneTest::Update(void)
{
	static float timer = 0;
	static float fov = this->camera->fov;
	static bool project = ProjectMode;
	static bool far_mode = false;

	timer += Time::DeltaTime();

	if (GetKeyboardTrigger(DIK_TAB) && this->camera->GetComponent<CameraFov>() != nullptr)
	{
		// 射影変換アニメーションの切り替え
		if (!GetKeyboardPress(DIK_LSHIFT))
		{
			project = !project;
			if (project)
				this->camera->SetState(TitleCamera::State::Normal);
			else
				this->camera->SetState(TitleCamera::State::Orthogonal);

			timer = 0.0f;

		}

		// カメラ移動のアニメーションの切り替え
		else
		{
			far_mode = !far_mode;
			if (far_mode)
			{
				this->title_poly->SetMixMode(1.0f);
				this->camera->SetState(TitleCamera::State::Far);
			}
			else
			{
				this->title_poly->SetMixMode(0.0f);
				this->camera->SetState(TitleCamera::State::Normal);
			}

		}

	}

	//this->logo->GetComponent<RectPolygon>()->SetOpacity(1.0f - this->title_poly->GetMixFactor());


	if (GetKeyboardPress(DIK_NUMPAD4))
		this->camera->fov += Deg2Rad(1.0f);
	if (GetKeyboardPress(DIK_NUMPAD6))
		this->camera->fov -= Deg2Rad(1.0f);
	if (GetKeyboardPress(DIK_NUMPAD8))
	{
		this->camera->GetComponent<CameraSmooth>()->distance -= 5.0f;
	}
	if (GetKeyboardPress(DIK_NUMPAD2))
	{
		this->camera->GetComponent<CameraSmooth>()->distance += 5.0f;
	}
	if (GetKeyboardPress(DIK_PGUP))
		this->camera->transform.position.z += 100.0f;
	if (GetKeyboardPress(DIK_PGDN))
		this->camera->transform.position.z += -100.0f;

	if (IsMouseCenterTriggered())
	{
		if (this->camera->GetComponent<CameraPlay>()->GetActive() == false)
		{
			//this->camera->GetComponent<CameraSmooth>()->SetActive(false);
			this->camera->GetComponent<CameraPlay>()->SetActive(true);
		}
	}

	if (GetKeyboardTrigger(DIK_F))
	{
		//this->camera->GetComponent<CameraSmooth>()->SetActive(true);
		this->camera->GetComponent<CameraPlay>()->SetActive(false);
	}


	if (GetKeyboardTrigger(DIK_RETURN))
	{
		this->test->GetComponent<ParticleSystem>()->SetDuration(0.3f);
	}
		/*this->title_poly->SetBehavior2();*/

	if (GetKeyboardTrigger(DIK_SPACE))
		this->player->SetDeath();

	int line = 0;
	//sprintf(GetDebugText(line++), "PolyCount: %d", this->polyCount);
	sprintf(GetDebugText(line++), "DeltaTime: %3.0fms", Time::DeltaTime()*1000);
	sprintf(GetDebugText(line++), "fov: %2.4f", this->camera->fov);
	sprintf(GetDebugText(line++), "view_angle: %2.1f", Rad2Deg(this->camera->fov));
	sprintf(GetDebugText(line++), "PlayerX: %5.1f", this->player->transform.position.x);
	sprintf(GetDebugText(line++), "PlayerY: %5.1f", this->player->transform.position.y);
	sprintf(GetDebugText(line++), "PlayerZ: %5.1f", this->player->transform.position.z);
	sprintf(GetDebugText(line++), "PlayerR: %5.1f", this->player->transform.getRotation().z);
	sprintf(GetDebugText(line++), "CameraX: %5.1f", this->camera->transform.position.x);
	sprintf(GetDebugText(line++), "CameraY: %5.1f", this->camera->transform.position.y);
	sprintf(GetDebugText(line++), "CameraZ: %5.1f", this->camera->transform.position.z);

	//sprintf(GetDebugText(line++), "MouseX: %3f", GetMousePos().x);
	//sprintf(GetDebugText(line++), "MouseY: %3f", GetMousePos().y);

	//sprintf(GetDebugText(line++), "ParticleCount: %5d", this->field_bk->GetComponent<ParticleSystem>()->GetParticleNum());
	//sprintf(GetDebugText(line++), "ParticleCount: %5d", this->test->GetComponent<ParticleSystem>()->GetParticleNum());

}

void SceneTest::Uninit(void)
{

	Bullet::Clear();

	for (int i = 0; i < 40; i++)
		strcpy(GetDebugText(i), "");

	RenderSpace::Delete("mini_map");


	ShowCursor(true);

	StopSound(BGM_GAME);
}