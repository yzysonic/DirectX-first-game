#include "SceneGame.h"
#include "SceneClear.h"
#include "SceneGameOver.h"
#include "FadeScreen.h"
#include "CameraSmooth.h"
#include "CameraShake.h"

/// public関数

// 初期化処理
void SceneGame::Init(void)
{
	// 口径食効果
	this->vignetting = new Object;
	this->vignetting->AddComponent<RectPolygon2D>("vignetting", Layer::MASK);

	// スコアUI
	this->scoreUI = new ScoreUI(5, SystemParameters::ResolutionX/2 - 300, SystemParameters::ResolutionY/2 - 30, "number", "game_score");
	this->scoreUI->setOffset(130, 0);

	// タイムUI
	const int x_offset = 35;
	this->timeUI[0] = new NumberUI(2, x_offset - SystemParameters::ResolutionX/2, SystemParameters::ResolutionY/2 - 30, "number", "game_time");
	this->timeUI[1] = new NumberUI(2, x_offset + 116 - SystemParameters::ResolutionX/2, SystemParameters::ResolutionY/2 - 30, "number");
	this->timeUI[0]->setOffset(180, 0);

	// 残機UI 
	this->liveUI = new LiveUI;
	this->liveUI->transform.position = Vector3(x_offset + 3 + Texture::Get("lives")->size.x/2 - SystemParameters::ResolutionX/2, SystemParameters::ResolutionY/2 - 70.f, 0.0f);

	// プレイヤー
	this->player = new Player;
	this->player->SetActive(false);

	// カメラ設定
	this->camera.reset(new Camera());
	this->camera->AddComponent<CameraSmooth>(this->player);
	this->camera->AddComponent<CameraShake>();
	this->camera->SetActive(false);
	this->camera->setBackColor(210, 210, 210, 255);
	Renderer::GetInstance()->setCamera(this->camera.get());

	// ミニマップ
	this->minimap = new MiniMap(200, 200, 11);
	this->minimap->SetPosition(Vector3(SystemParameters::ResolutionX / 2.0f - 150.0f, -SystemParameters::ResolutionY / 2.0f + 150.0f, 0.0f));
	this->minimap->SetPlayer(this->player);
	this->minimap->zoom = 0.3f;

	// イベントバインド
	this->player->injury += [&]
	{
		// 揺れエフェクト
		this->camera->GetComponent<CameraShake>()->SetActive(true);
		this->minimap->Shake();

		// 残機表示の更新
		this->liveUI->GetComponent<RectPolygon2D>()->setPattern(this->player->hp - 1);

		// HPが１になった時の警告
		if (this->player->hp == 1)
		{
			this->liveUI->SetLowLive(true);
			PlayBGM(SE_LOW_HP);
			SetVolume(SE_LOW_HP, -1800);
		}
	};

	// ゲームで使う変数
	this->score = 0;
	this->timer = GameTime;
	this->polyCount = 0;

	for (int i = 0; i < ENEMY_MAX; i++)
		this->enemy[i] = NULL;

	// 背景ポリゴン生成
	for (int i = 0; i < GAME_POLY_MAX; i++)
	{
		this->polyList[i] = new PolygonElement;
		this->polyList[i]->transform.position.x = Randomf(-FIELD_RANG_X, FIELD_RANG_X);
		this->polyList[i]->transform.position.y = Randomf(-FIELD_RANG_Y, FIELD_RANG_Y);
		this->polyList[i]->targetOpacity = 0.7f;
		this->polyList[i]->targetScale = Vector3(0.1f, 0.1f, 1.0f);
		int min = (int)(200 * powf((this->polyList[i]->transform.position.z + 100) / 400, 2.0f));
		this->polyList[i]->GetComponent<RectPolygon>()->SetColor(Color(Random(min, 255), Random(min, 255), Random(min, 255), 0));

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
void SceneGame::Update(void)
{
	(this->*pUpdate)();
}

// 終了処理
void SceneGame::Uninit(void)
{
	GameManager::Var<int>("score") = this->score;

	StopSound(BGM_GAME);

	delete this->minimap;

	for (auto &enemy : this->enemy)
	{
		SafeDelete<Enemy>(enemy);
	}
	delete this->player;
	delete this->vignetting;
	delete this->liveUI;
	delete this->scoreUI;
	delete this->timeUI[0];
	delete this->timeUI[1];
	

	for (int i = 0; i < this->polyCount; i++)
		delete (this->polyList[i]);

	

	Renderer::GetInstance()->setCamera(nullptr);
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
		this->player->SetActive(true);
		this->camera->SetActive(true);
		SceneGame::pUpdate = &SceneGame::update_main;
	}
}

void SceneGame::update_main(void)
{
	// 敵の生成
	swapEnemy();

	for (auto &enemy : this->enemy)
	{
		if (enemy == nullptr)
			continue;

		if (enemy->hp == 0)
		{
			// スコア更新
			this->addGameScore(300);
			this->scoreUI->SetScore(this->score);
			this->minimap->RemoveEnemy(enemy);
			Object::Destroy(enemy);
		}
	}

	// プレイヤーの移動制限
	Vector3 &playerPos = this->player->transform.position;
	if (playerPos.x < -FIELD_RANG_X)
		playerPos.x = -FIELD_RANG_X;
	if (playerPos.x > FIELD_RANG_X)
		playerPos.x = FIELD_RANG_X;
	if (playerPos.y < -FIELD_RANG_Y)
		playerPos.y = -FIELD_RANG_Y;
	if (playerPos.y > FIELD_RANG_Y)
		playerPos.y = FIELD_RANG_Y;


	// カウントダウン更新
	this->timer -= Time::DeltaTime();

	// カウントダウン表示更新
	this->timeUI[0]->setNumber((int)(this->timer*100)%100);
	this->timeUI[1]->setNumber((int)this->timer);
	if (this->timer < 10)
	{
		UCHAR p = (UCHAR)(255 * fabsf(sinf(this->timer*PI / 0.7f)));
		Color c;
		c.setRGBA(255, p, p, 255);
		this->timeUI[0]->SetColor(c);
		this->timeUI[1]->SetColor(c);
	}

	// シーン遷移→クリアシーン
	if (this->timer < 0)
	{
		GameManager::SetScene(new SceneClear);
		return;
	}

	 //シーン遷移→ゲームオーバー
	if (this->player->hp == 0)
	{
		GameManager::SetScene(new SceneGameOver);
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
		for (auto &enemy : this->enemy)
		{
			if (enemy == nullptr)
			{
				enemy = new Enemy;

				enemy->target = &this->player->transform;
				enemy->transform.position = Vector3(Randomf(-FIELD_RANG_X + 500, FIELD_RANG_X - 500), Randomf(-FIELD_RANG_Y + 500, FIELD_RANG_Y - 500), 0.0f);
				this->minimap->SetEnemy(enemy);

				break;
			}
		}
		timer = 0;
	}

	timer += Time::DeltaTime();
}
