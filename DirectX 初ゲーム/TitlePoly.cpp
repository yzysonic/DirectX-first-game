#include "TitlePoly.h"

TitlePoly::TitlePoly(void)
{
	D3DLOCKED_RECT rect;
	D3DSURFACE_DESC desc;
	Texture* title_mask = nullptr;

	// マスク初期化
	title_mask = Texture::Get("title_mask");
	title_mask->pDXTex->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
	title_mask->pDXTex->GetLevelDesc(0, &desc);

	// テクスチャからアルファチャンネルの値を取得し、対応する位置情報をマップに入れる
	std::vector<Data> map[256];

	for (UINT h = 0; h < desc.Height; h++)
	{
		Color* pixel_data = (Color*)((BYTE*)rect.pBits + rect.Pitch*h);
		Data data;

		for (UINT w = 0; w < desc.Width; w++)
		{
			data.pos = Vector2((float)w / desc.Width, (float)h / desc.Height);
			data.color = pixel_data[w];
			map[pixel_data[w].a].push_back(data);
		}
	}

	title_mask->pDXTex->UnlockRect(0);

	float sum = 0.0f;
	std::vector<float> p_list;

	for (int i = 1; i < 256; i++)
	{
		auto list = map + i;
		if (list->size() > 0)
		{
			p_list.push_back((float)i);
			this->data_map.push_back(*list);
			sum += i;
		}
	}

	sum -= p_list.back();
	p_list.back() *= 1000.0f;
	sum += p_list.back();

	// ランダムテーブルの初期化
	this->random_table.Init(p_list);

	// パーティクル初期化
	auto particle =	AddComponent<ParticleSystem>(10000, this);
	particle->SetDuration(-1.0f);
	particle->emission_rate = 43000.0f;


	auto mask = AddComponent<RectPolygon>("title_mask");
	mask->SetOpacity(0.1f);
	mask->SetActive(false);

	transform.position.x = 250.0f;
	transform.position.y = -700.0f;
	transform.position.z = 10.0f;
	transform.scale *= 100.0f;

	this->size = GetComponent<RectPolygon>()->pTexture->size;
	this->size.x *= transform.scale.x;
	this->size.y *= transform.scale.y;


	this->camera = Renderer::GetInstance()->getCamera("default");
	this->mix_mode = false;
	this->mix_factor = 0.0f;
	this->scale = PolySize;
}

void TitlePoly::MakeElement(ParticleElement ** elements, UINT* pitch, UINT number)
{
	*elements = new Element[number];
	*pitch = sizeof(Element);
}

void TitlePoly::Init(ParticleElement & element)
{
	auto &e = (Element&)(element);

	int i = this->random_table.Next();
	auto& data = this->data_map[i][Random(0, this->data_map[i].size() - 1)];
	Vector2 pos = data.pos + Vector2(Randomf(0.0f, 0.01f), Randomf(0.0f, 0.01f));

	e.transform.position.x = (pos.x -0.5f) * size.x;
	e.transform.position.y = (0.5f -pos.y) * size.y;
	//e.transform.position.z = Randomf(-200.0f, 200.0f);
	e.transform.position.z = Randomf(-500.0f, -260.0f);
	e.transform.position += transform.position;
	e.fix_pos = e.transform.position;

	e.fix_color1 = e.color = Color(Random(0, 255), Random(0, 255), Random(0, 255), 255);
	e.fix_color2 = data.color;
}

void TitlePoly::Update(ParticleElement & element)
{
	auto &e = (Element&)(element);

	if (e.timer.TimeUp())
	{
		e.dir = Vector3(Randomf(-1, 1), Randomf(-1, 1), 0.0f)*30.0f;
		e.timer.Reset(Randomf(0.2f, 0.7f));
	}

	e.transform.position += e.dir*Time::DeltaTime();
	e.timer.Step();

	e.transform.scale = this->scale*Vector3::one;

	e.color = LerpC(e.fix_color1, e.fix_color2, this->mix_factor*0.6f);

}

void TitlePoly::Update(void)
{
	if (this->mix_mode)
	{
		this->mix_factor = (-this->camera->transform.position.z - 250.0f) / 6700.0f;

		this->scale = PolySize*Lerpf(1.0f, 3.0f, this->mix_factor);

		if (fabsf(this->mix_factor - this->target_mix_factor) <= 0.001f)
		{
			this->mix_factor = this->target_mix_factor;
			this->mix_mode = false;
		}

		this->camera->setBackColor(LerpC(Color(210, 210, 210, 255), Color(50, 50, 50, 255), this->mix_factor));
	}
}

void TitlePoly::SetMixMode(float target_mix_factor)
{
	this->target_mix_factor = target_mix_factor;
	this->mix_mode = true;
}

float TitlePoly::GetMixFactor(void)
{
	return this->mix_factor;
}
