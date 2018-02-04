#include "SceneTest.h"
#include "Time.h"
#include "Core/Renderer.h"
#include "Core\RenderSpace.h"
#include "FadeScreen.h"
#include "CameraSmooth.h"
#include "CameraFov.h"
#include "CameraShake.h"
#include "CameraPlay.h"

#define testMax 2000
#define ProjectMode 0

void SceneTest::Init(void)
{

	this->player = /*AddObject*/(new Player);
	//this->enemy = new Enemy;
	//enemy->target = &this->player->transform;
	//enemy->transform.position = Vector3(30, 30, 0);

	this->camera = /*AddObject*/(new Camera());
	this->camera->AddComponent<CameraSmooth>(this->player);
	this->camera->AddComponent<CameraFov>();
	this->camera->AddComponent<CameraShake>();
	this->camera->AddComponent<CameraPlay>();
	this->camera->setBackColor(210, 210, 210, 255);

	//this->camera->GetComponent<CameraFov>()->target_fov = Deg2Rad(1.0f);
	//this->camera->GetComponent<CameraFov>()->SetActive(true);

	Renderer::GetInstance()->setCamera(this->camera);

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



	 //背景ポリゴン生成
	//this->polyCount = 0;
	//this->polyList.resize(PolyMax);
	//for (auto& poly : this->polyList)
	//{
	//	poly = new PolygonElement;
	//	poly->transform.position.x = Randomf(-FieldRangeX, FieldRangeX);
	//	poly->transform.position.y = Randomf(-FieldRangeY, FieldRangeY);
	//	poly->targetOpacity = 0.7f;
	//	poly->targetScale = Vector3(0.1f, 0.1f, 1.0f);
	//	this->polyCount++;
	//}
	this->PolyMax = 200;
	this->test = /*AddObject*/(new Object);
	auto particle = 
	this->test->AddComponent<ParticleSystem>(PolyMax);
	particle->GetBehavior<ParticleDefaultBehavior>()->start_color = Color(0, 0, 255, 255);
	particle->SetDuration(0.5f);
	particle->emission_rate = 100.0f;
	//this->field_bk = new FieldPolygon(PolyMax);
	this->polyCount = PolyMax;

	SetVolume(BGM_GAME, -1800);
	FadeScreen::FadeIn(Color::black, 0.0f);
	//PlayBGM(BGM_GAME);
}

void SceneTest::Update(void)
{
	static float timer = 0;
	static float fov = this->camera->fov;
	static bool project = ProjectMode;

	timer += Time::DeltaTime();

	if (GetKeyboardTrigger(DIK_TAB) && this->camera->GetComponent<CameraFov>() != nullptr)
	{
		project = !project;

		if (project)
			this->camera->GetComponent<CameraFov>()->target_fov = Deg2Rad(110.0f);
		else
			this->camera->GetComponent<CameraFov>()->target_fov = Deg2Rad(1.0f);

		this->camera->GetComponent<CameraFov>()->speed = 3.0f;
		this->camera->GetComponent<CameraFov>()->SetActive(true);

		timer = 0.0f;
	}

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
		this->camera->fov += 0.01f;
	if (GetKeyboardPress(DIK_PGDN))
		this->camera->fov -= 0.01f;
	if (GetKeyboardPress(DIK_HOME))
		this->player->transform.position.z += 10.0f;
	if(GetKeyboardPress(DIK_END))
		this->player->transform.position.z += -10.0f;

	if (IsMouseCenterTriggered())
	{
		if (this->camera->GetComponent<CameraPlay>()->GetActive() == false)
		{
			this->camera->GetComponent<CameraSmooth>()->SetActive(false);
			this->camera->GetComponent<CameraPlay>()->SetActive(true);
		}
	}

	if (GetKeyboardTrigger(DIK_F))
	{
		this->camera->GetComponent<CameraSmooth>()->SetActive(true);
		this->camera->GetComponent<CameraPlay>()->SetActive(false);
	}


	if (GetKeyboardTrigger(DIK_RETURN))
		this->test->GetComponent<ParticleSystem>()->SetDuration(0.5f);

	int line = 0;
	sprintf(GetDebugText(line++), "PolyCount: %d", this->polyCount);
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

	sprintf(GetDebugText(line++), "MouseX: %3f", GetMousePos().x);
	sprintf(GetDebugText(line++), "MouseY: %3f", GetMousePos().y);

	//sprintf(GetDebugText(line++), "ParticleCount: %5d", this->field_bk->GetComponent<ParticleSystem>()->GetParticleNum());
	sprintf(GetDebugText(line++), "ParticleCount: %5d", this->test->GetComponent<ParticleSystem>()->GetParticleNum());

}

void SceneTest::Uninit(void)
{
	//delete(this->camera);
	//delete(this->player);
	//delete(this->enemy);
	//delete(this->mini_map);

	//for (auto poly : this->polyList)
	//	delete poly;

	Bullet::Clear();

	for (int i = 0; i < 40; i++)
		strcpy(GetDebugText(i), "");

	RenderSpace::Delete("mini_map");
	Renderer::GetInstance()->setCamera(nullptr);


	ShowCursor(true);

	StopSound(BGM_GAME);
}