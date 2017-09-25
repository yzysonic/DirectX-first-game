#include "SceneTest.h"
#include "Time.h"
#include "Renderer.h"

#define testMax 2000
#define ProjectMode 0

void SceneTest::init(void)
{
	this->player = new Player;
	this->camera = new SmoothCamera(this->player->getTransform());

	this->testCount = 0;

	Renderer::GetInstance()->setCamera(this->camera);
	this->camera->backColor = ColorRGBA(210,210,210,255);
	this->camera->fov = ProjectMode;

	//ShowCursor(false);

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
	sprintf(GetDebugText(line++), "PlayerR: %5.1f", this->player->getTransform()->rotation.z);
	sprintf(GetDebugText(line++), "CameraX: %5.1f", this->camera->getTransform()->position.x);
	sprintf(GetDebugText(line++), "CameraY: %5.1f", this->camera->getTransform()->position.y);
	sprintf(GetDebugText(line++), "CameraZ: %5.1f", this->camera->getTransform()->position.z);
	sprintf(GetDebugText(line++), "MouseX: %3f", GetMousePos().x);
	sprintf(GetDebugText(line++), "MouseY: %3f", GetMousePos().y);
	sprintf(GetDebugText(line++), "PADLx: %2.1f", GetPadLX());
	sprintf(GetDebugText(line++), "PADLx: %2.1f", GetPadLY());
	sprintf(GetDebugText(line++), "PADRx: %2.1f", GetPadRX());
	sprintf(GetDebugText(line++), "PADRy: %2.1f", GetPadRY());
	sprintf(GetDebugText(line++), "BUTTON: %d", IsButtonPressed(0, BUTTON_R1));
	sprintf(GetDebugText(line++), "BUTTON: %d", IsButtonPressed(0, BUTTON_L2));
	sprintf(GetDebugText(line++), "BUTTON: %d", IsButtonPressed(0, BUTTON_R2));
	sprintf(GetDebugText(line++), "BUTTON: %d", IsButtonPressed(0, BUTTON_SHARE));
	sprintf(GetDebugText(line++), "BUTTON: %d", IsButtonPressed(0, BUTTON_OPTIONS));

}

void SceneTest::uninit(void)
{
	delete(this->camera);
	delete(this->player);

	Bullet::Clear();

	for (int i = 0; i < 20; i++)
		strcpy(GetDebugText(i), "");

	ShowCursor(true);

	StopSound(BGM_GAME);
}