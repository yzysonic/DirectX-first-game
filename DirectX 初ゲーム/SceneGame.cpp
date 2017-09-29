#include "SceneGame.h"
#include "GameManager.h"


/// public関数

// 初期化処理
void SceneGame::init(void)
{
	// 口径食効果
	this->vignetting = new Object;
	this->vignetting->setPolygon(Layer::MASK, TEX_VIGNETTING, RendererType::UI);

	// スコアUI
	this->scoreUI = new NumberUI(5, SCREEN_CENTER_X - 300, 30 - SCREEN_CENTER_Y, TEX_NUMBER, TEX_GAME_SCORE);
	this->scoreUI->setOffset(130, 0);

	// タイムUI
	const int x_offset = 35;
	this->timeUI[0] = new NumberUI(2, x_offset - SCREEN_CENTER_X, 30 - SCREEN_CENTER_Y, TEX_NUMBER, TEX_GAME_TIME);
	this->timeUI[1] = new NumberUI(2, x_offset + 116 - SCREEN_CENTER_X, 30 - SCREEN_CENTER_Y, TEX_NUMBER);
	this->timeUI[0]->setOffset(180, 0);

	// 残機UI 
	this->liveUI = new Object;
	this->liveUI->setPolygon(Layer::UI_00, TEX_LIFES, RendererType::UI);
	this->liveUI->getTransform()->position = Vector3(x_offset + 3 + GetTexture(TEX_LIFES)->size.x/2 - SCREEN_CENTER_X, 70 - SCREEN_CENTER_Y, 0.0f);

	// プレイヤー
	this->player = new Player;
	this->player->setActive(false);

	// カメラ設定
	this->camera = new SmoothCamera(this->player->getTransform(), Vector3(0, -50, -5));
	this->camera->setActive(false);
	this->camera->setBackColor(210, 210, 210, 255);
	this->camera->fov = 1;
	Renderer::GetInstance()->setCamera(this->camera);

	// ゲームで使う変数
	this->score = 0;
	this->timer = 60.0f;
	this->polyCount = 0;

	for (int i = 0; i < ENEMY_MAX; i++)
		this->enemy[i] = NULL;

	// 背景ポリゴン生成
	for (int i = 0; i < GAME_POLY_MAX; i++)
	{
		this->polyList[i] = new PolygonElement;
		this->polyList[i]->getTransform()->position.x = Randomf(-FIELD_RANG_X, FIELD_RANG_X);
		this->polyList[i]->getTransform()->position.y = Randomf(-FIELD_RANG_Y, FIELD_RANG_Y);
		this->polyList[i]->getTransform()->position.z = Randomf(0.01f, 3);
		this->polyList[i]->targetOpacity = 0.7f;
		this->polyCount++;
	}

	// フェイトイン効果
	FadeScreen::FadeIn(Color::white, 0.7f);
	
	// BGMを再生
	SetVolume(BGM_GAME, -1800);
	PlayBGM(BGM_GAME);

	// ゲーム状態→フェイト処理完了待ち
	SceneGame::pUpdate = &SceneGame::update_fadeWait;
}

// グローバル更新処理
void SceneGame::update(void)
{
	(this->*pUpdate)();
}

// 終了処理
void SceneGame::uninit(void)
{
	GameManager::GetInstance()->score = this->score;

	StopSound(BGM_GAME);

	delete this->vignetting;
	delete this->liveUI;
	delete this->scoreUI;
	delete this->timeUI[0];
	delete this->timeUI[1];
	delete this->camera;
	delete this->player;

	for (int i = 0; i < this->polyCount; i++)
		delete (this->polyList[i]);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if(this->enemy[i] != nullptr)
			delete this->enemy[i];
	}

	Bullet::Clear();
}

// スコアの取得
int SceneGame::getGameScore(void)
{
	return this->score;
}

void SceneGame::addGameScore(int score)
{
	this->score += score;
}


/// private関数

// ファイト処理完了待ち
void SceneGame::update_fadeWait(void)
{
	//if (FadeFinished())
	{
		this->player->setActive(true);
		this->camera->setActive(true);
		SceneGame::pUpdate = &SceneGame::update_main;
	}
}

void SceneGame::update_main(void)
{
	// 敵の生成
	swapEnemy();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (this->enemy[i] == nullptr)
			continue;

		if (this->enemy[i]->hp == 0)
		{
			this->addGameScore(300);
			delete this->enemy[i];
			this->enemy[i] = nullptr;
		}
	}
	

	// プレイヤーの移動制限
	Vector3 &playerPos = this->player->getTransform()->position;
	if (playerPos.x < -FIELD_RANG_X)
		playerPos.x = -FIELD_RANG_X;
	if (playerPos.x > FIELD_RANG_X)
		playerPos.x = FIELD_RANG_X;
	if (playerPos.y < -FIELD_RANG_Y)
		playerPos.y = -FIELD_RANG_Y;
	if (playerPos.y > FIELD_RANG_Y)
		playerPos.y = FIELD_RANG_Y;

	// 残機表示の更新
	this->liveUI->getPolygon()->setPattern(this->player->hp-1);

	// カウントダウン更新
	this->timer -= Time::DeltaTime();

	// カウントダウン表示更新
	this->timeUI[0]->setNumber((int)(this->timer*100)%100);
	this->timeUI[0]->setNumber((int)this->timer);

	// スコア表示更新
	this->scoreUI->setNumber(this->score);

	// シーン遷移→クリアシーン
	if (this->timer < 0)
	{
		GameManager::SetScene(SceneName::CLEAR);
		return;
	}

	// シーン遷移→ゲームオーバー
	if (this->player->hp == 0)
	{
		GameManager::SetScene(SceneName::GAMEOVER);
		return;
	}

}

void SceneGame::swapEnemy(void)
{
	static float timer = 0;

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_E))
		timer = 10;
#endif

	if (timer > 3.0f)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (this->enemy[i] == NULL)
			{
				Enemy* enemy = 
				this->enemy[i] = new Enemy;

				enemy->target = this->player->getTransform();
				enemy->getTransform()->position = Vector3(Randomf(-FIELD_RANG_X+500, FIELD_RANG_X-500), Randomf(-FIELD_RANG_Y+500, FIELD_RANG_X-500), 0.0f);

				break;
			}
		}
		timer = 0;
	}

	timer += Time::DeltaTime();
}
