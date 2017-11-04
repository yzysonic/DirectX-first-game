#include "SceneTest.h"
#include "Time.h"
#include "Core/Renderer.h"

#define testMax 2000
#define ProjectMode 0

void SceneTest::init(void)
{
	this->player = new Player;
	this->enemy = new Enemy;
	enemy->target = this->player->getTransform();
	enemy->getTransform()->position = Vector3(30, 30, 0);
	this->camera = new SmoothCamera(this->player->getTransform());

	this->testCount = 0;

	Renderer::GetInstance()->setCamera(this->camera);
	this->camera->setBackColor(210,210,210,255);
	this->camera->fov = ProjectMode;


	SetVolume(BGM_GAME, -1800);
	//PlayBGM(BGM_GAME);
}

void SceneTest::update(void)
{
	static float timer = 0;
	static int &i = this->testCount;
	static bool bReset = false;
	static float fov = this->camera->fov;
	static int project = ProjectMode;

	timer += Time::DeltaTime();

	this->camera->fov = Lerpf(this->camera->fov, (float)project, Time::DeltaTime()*5);

	if (GetKeyboardPress(DIK_RETURN))
	{
		bReset = true;
	}

	if (GetKeyboardTrigger(DIK_TAB))
	{
		project = !project;
	}


	int line = 0;
	sprintf(GetDebugText(line++), "ObjectCount: %d", i);
	sprintf(GetDebugText(line++), "DeltaTime: %3.0fms", Time::DeltaTime()*1000);
	sprintf(GetDebugText(line++), "fov: %2.1f", this->camera->fov);
	sprintf(GetDebugText(line++), "PlayerX: %5.1f", this->player->getTransform()->position.x);
	sprintf(GetDebugText(line++), "PlayerY: %5.1f", this->player->getTransform()->position.y);
	sprintf(GetDebugText(line++), "PlayerZ: %5.1f", this->player->getTransform()->position.z);
	sprintf(GetDebugText(line++), "PlayerR: %5.1f", this->player->getTransform()->getRotation().z);
	sprintf(GetDebugText(line++), "CameraX: %5.1f", this->camera->getTransform()->position.x);
	sprintf(GetDebugText(line++), "CameraY: %5.1f", this->camera->getTransform()->position.y);
	sprintf(GetDebugText(line++), "CameraZ: %5.1f", this->camera->getTransform()->position.z);
	sprintf(GetDebugText(line++), "MouseX: %3f", GetMousePos().x);
	sprintf(GetDebugText(line++), "MouseY: %3f", GetMousePos().y);

}

void SceneTest::uninit(void)
{
	delete(this->camera);
	delete(this->player);
	delete(this->enemy);

	Bullet::Clear();

	for (int i = 0; i < 40; i++)
		strcpy(GetDebugText(i), "");

	ShowCursor(true);

	StopSound(BGM_GAME);
}