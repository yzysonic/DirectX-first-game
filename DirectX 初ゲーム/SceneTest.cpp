#include "SceneTest.h"
#include "Time.h"
#include "Core/Renderer.h"
#include "Core\RenderSpace.h"

#define testMax 2000
#define ProjectMode 0

void SceneTest::init(void)
{

	this->player = new Player;
	this->enemy = new Enemy;
	enemy->target = this->player->getTransform();
	enemy->getTransform()->position = Vector3(30, 30, 0);
	this->camera = new SmoothCamera(this->player->getTransform());

	this->mini_map = new MiniMap(200, 200, 11);
	this->mini_map->SetPosition(Vector3(SystemParameters::ResolutionX / 2.0f - 150.0f, -SystemParameters::ResolutionY / 2.0f + 150.0f, 0.0f));
	this->mini_map->SetPlayer(this->player);
	this->mini_map->SetEnemy(this->enemy);

	this->test = new Object;
	this->test->setPolygon();
	this->test->getTransform()->position.z = 90.0f;
	this->test->getTransform()->scale = Vector3::one*0.1f;

	this->polyCount = 0;

	Renderer::GetInstance()->setCamera(this->camera);
	this->camera->setBackColor(210,210,210,255);
	this->camera->fov = ProjectMode;
	this->camera->distance = 100.0f;

	// ”wŒiƒ|ƒŠƒSƒ“¶¬
	for (int i = 0; i < PolyMax; i++)
	{
		this->polyList[i] = new PolygonElement;
		this->polyList[i]->getTransform()->position.x = Randomf(-FieldRangeX, FieldRangeX);
		this->polyList[i]->getTransform()->position.y = Randomf(-FieldRangeY, FieldRangeY);
		this->polyList[i]->getTransform()->position.z = Randomf(0.01f, 3.0f);
		this->polyList[i]->targetOpacity = 0.7f;
		this->polyList[i]->targetScale = Vector3(0.1f, 0.1f, 1.0f);
		this->polyCount++;
	}

	SetVolume(BGM_GAME, -1800);
	//PlayBGM(BGM_GAME);
}

void SceneTest::update(void)
{
	static float timer = 0;
	static float fov = this->camera->fov;
	static int project = ProjectMode;

	timer += Time::DeltaTime();

	this->camera->fov = Lerpf(this->camera->fov, (float)project, Time::DeltaTime()*5);

	if (GetKeyboardTrigger(DIK_TAB))
	{
		project = !project;
	}

	if (GetKeyboardPress(DIK_NUMPAD4))
		this->camera->view_angle += Deg2Rad(1.0f);
	if (GetKeyboardPress(DIK_NUMPAD6))
		this->camera->view_angle -= Deg2Rad(1.0f);
	if (GetKeyboardPress(DIK_NUMPAD8))
		this->camera->distance -= 5.0f;
	if (GetKeyboardPress(DIK_NUMPAD2))
		this->camera->distance += 5.0f;

	if (GetKeyboardTrigger(DIK_RETURN))
		this->mini_map->Shake();

	if (player->shake_flag)
	{
		camera->Shake();
		mini_map->Shake();
		player->shake_flag = false;
	}

	this->test->getTransform()->rotate(0.0f, 0.1f, 0.0f);


	int line = 0;
	sprintf(GetDebugText(line++), "PolyCount: %d", this->polyCount);
	sprintf(GetDebugText(line++), "DeltaTime: %3.0fms", Time::DeltaTime()*1000);
	sprintf(GetDebugText(line++), "fov: %2.1f", this->camera->fov);
	sprintf(GetDebugText(line++), "view_angle: %2.1f", Rad2Deg(this->camera->view_angle));
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
	delete(this->test);
	delete(this->mini_map);

	for (int i = 0; i < this->polyCount; i++)
		delete (this->polyList[i]);

	Bullet::Clear();

	for (int i = 0; i < 40; i++)
		strcpy(GetDebugText(i), "");

	RenderSpace::Delete("mini_map");

	ShowCursor(true);

	StopSound(BGM_GAME);
}