#include "ParticleSystem.h"

ParticleDefaultBehavior ParticleSystem::default_behavior;


ParticleSystem::ParticleSystem(UINT particle_max, IParticleBehavior* behavior) : Drawable(Layer::DEFAULT, "default")
{
	this->behavior = behavior;
	this->duration = 5.0f;
	this->emission_rate = 10.0f;
	this->particle_max = particle_max;
	this->particle_num = 0;
	this->loop = false;

	this->behavior->MakeElement(&this->elements, &this->pitch, particle_max);
	InitDraw();
}

ParticleSystem::~ParticleSystem(void)
{
	delete[] this->elements;
	UninitDraw();
}

void ParticleSystem::Update(void)
{
	ParticleElement* element = nullptr;

	// パーティクルシステムのループ処理
	if (this->timer_duration.TimeUp())
	{
		// 周期のリセット
		if(this->loop)
			this->timer_duration.Reset(this->duration);
	}
	else
	{
		// パーティクル生成処理
		if (this->timer_emission.TimeUp())
		{
			float emission_deltatime = 1.0f / this->emission_rate;
			int emission_num = (int)(this->timer_emission.Elapsed() / emission_deltatime);

			element = this->elements;
			for (UINT i = 0; (i < this->particle_max) && (this->particle_num < this->particle_max) && (emission_num > 0); i++, element = (ParticleElement*)((BYTE*)this->elements + i*this->pitch))
			{
				if (element->active == false)
				{
					element->transform.position = this->object->transform.position;
					this->behavior->Init(*element);
					element->active = true;
					this->particle_num++;
					emission_num--;
				}
			}
			this->timer_emission.Reset(emission_deltatime);
		}
	}

	// パーティクル更新処理
	element = this->elements;
	for (UINT i = 0; i < this->particle_max; i++, element = (ParticleElement*)((BYTE*)this->elements + i*this->pitch))
	{
		if (element->active)
		{
			this->behavior->Update(*element);
			if (element->active == false)
				this->particle_num--;
		}
	}

	this->timer_duration.Step();
	this->timer_emission.Step();
}

void ParticleSystem::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();
	VtxParticleInstance* pInstance;
	D3DXHANDLE const_handle;

	// 更新処理
	Update();

	// ビューマトリクスの設定
	const_handle = this->vshader->pConstantTable->GetConstantByName(NULL, "mtxView");
	this->vshader->pConstantTable->SetMatrix(pDevice, const_handle, &Renderer::GetInstance()->getCamera()->getViewMatrix(false));

	// プロジェクションマトリクスの設定
	const_handle = this->vshader->pConstantTable->GetConstantByName(NULL, "mtxProjection");
	this->vshader->pConstantTable->SetMatrix(pDevice, const_handle, &Renderer::GetInstance()->getCamera()->getProjectionMatrix(false));
	
	// インスタンスバッファの更新
	this->pInstanceBuff->Lock(0, 0, (void**)&pInstance, D3DLOCK_DISCARD);

	for (UINT i = 0; i<this->particle_max; i++)
	{
		auto element = (ParticleElement*)((BYTE*)this->elements + i*this->pitch);
		
		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		D3DXMatrixScaling(&mtxScl, element->transform.scale.x, element->transform.scale.y, element->transform.scale.z);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, element->transform.getRotation().y, element->transform.getRotation().x, element->transform.getRotation().z);
		D3DXMatrixTranslation(&mtxTranslate, element->transform.position.x, element->transform.position.y, element->transform.position.z);

		// ワールドマトリクスの設定
		pInstance->mtx_world = mtxScl*mtxRot*mtxTranslate;
		pInstance->color = element->color;
		pInstance++;
	}

	this->pInstanceBuff->Unlock();


	// 描画設定
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | this->particle_max);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	pDevice->SetStreamSource(0, this->pGeometryBuff, 0, sizeof(VtxParticleGeometry));
	pDevice->SetStreamSource(1, this->pInstanceBuff, 0, sizeof(VtxParticleInstance));

	pDevice->SetVertexDeclaration(this->decl);
	pDevice->SetVertexShader(this->vshader->pD3DShader);
	pDevice->SetIndices(this->pIndexBuff);

	pDevice->SetTexture(0, NULL);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetVertexShader(NULL);
}

HRESULT ParticleSystem::OnLostDevice(void)
{
	SafeRelease(this->pInstanceBuff);
	return S_OK;
}

HRESULT ParticleSystem::OnResetDevice(void)
{
	if (this->pInstanceBuff != nullptr)
		return E_FAIL;

	return Direct3D::GetDevice()->CreateVertexBuffer(
		sizeof(VtxParticleInstance) * this->particle_max,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_DYNAMIC,			// 頂点バッファの使用法　
		0,							// 使用する頂点フォーマット
		D3DPOOL_DEFAULT,			// リソースのバッファを保持するメモリクラスを指定
		&this->pInstanceBuff,		// 頂点バッファインターフェースへのポインタ
		NULL);
}

void ParticleSystem::InitDraw(void)
{
	auto pDevice = Direct3D::GetDevice();
	void* ptr = nullptr;

	this->particle_max = particle_max;

	// ポリゴンのデータ
	VtxParticleGeometry vtx[4] = {
		{ Vector3(-0.5f, 0.5f, 0.0f) },
		{ Vector3(0.5f, 0.5f, 0.0f) },
		{ Vector3(-0.5f,-0.5f, 0.0f) },
		{ Vector3(0.5f,-0.5f, 0.0f) }
	};


	// 頂点定義
	const D3DVERTEXELEMENT9 decl_element[] =
	{
		// Stream 0
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

		// Stream 1
		{ 1, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
		{ 1, 64, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration(decl_element, &decl);

	// インデックスバッファを作る
	WORD index[6] = { 0, 1, 2, 2, 1, 3 };
	pDevice->CreateIndexBuffer(
		sizeof(index),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&this->pIndexBuff,
		NULL);

	this->pIndexBuff->Lock(0, 0, &ptr, 0);
	memcpy(ptr, index, sizeof(index));
	this->pIndexBuff->Unlock();

	// ジオメトリバッファを作る
	pDevice->CreateVertexBuffer(
		sizeof(vtx),			// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
		0,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,		// リソースのバッファを保持するメモリクラスを指定
		&this->pGeometryBuff,	// 頂点バッファインターフェースへのポインタ
		NULL);


	// ポリゴンデータをジオメトリバッファへコピーする
	this->pGeometryBuff->Lock(0, 0, &ptr, 0);
	memcpy(ptr, vtx, sizeof(vtx));
	this->pGeometryBuff->Unlock();


	// インスタンスバッファを作る
	pDevice->CreateVertexBuffer(
		sizeof(VtxParticleInstance) * this->particle_max,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_DYNAMIC,			// 頂点バッファの使用法　
		0,							// 使用する頂点フォーマット
		D3DPOOL_DEFAULT,			// リソースのバッファを保持するメモリクラスを指定
		&this->pInstanceBuff,		// 頂点バッファインターフェースへのポインタ
		NULL);

	// シェーダの初期化
	this->vshader = VertexShader::Get("InstancingVS");
}

void ParticleSystem::UninitDraw(void)
{
	SafeRelease(this->pGeometryBuff);
	SafeRelease(this->pInstanceBuff);
}


void ParticleSystem::SetMaxNum(UINT value)
{
	this->particle_max = value;
	OnLostDevice();
	OnResetDevice();
}

void ParticleSystem::SetBehavior(IParticleBehavior * behavior)
{
	this->behavior = behavior;
}

void ParticleSystem::SetDuration(float value)
{
	this->duration = value;
	this->timer_duration.Reset(value);
}

UINT ParticleSystem::GetParticleNum(void)
{
	return this->particle_num;
}

ParticleDefaultBehavior::ParticleDefaultBehavior(void)
{
	this->lifetime = 1.0f;
	this->damping = 0.0f;

	this->start_speed = 10.0f;
	this->start_size = 10.0f;
	this->start_opacity = 1.0f;
	this->start_color = Color(255, 255, 255, 255);

	this->end_size = 5.0f;
	this->end_opacity = 0.0f;
	this->end_color = Color(255, 255, 255, 255);
}

void ParticleDefaultBehavior::Init(ParticleElement & element)
{
	element.timer.Reset(this->lifetime);
	element.color = this->start_color;
	element.transform.scale = this->start_size*Vector3::one;

	// 補間関数の初期化
	this->timing_func["size"] = Lerpf;
	this->timing_func["opacity"] = Lerpf;


	// 球状からランダムに発射
	float theta = Randomf(-2.0f*PI, 2.0f*PI);
	float phi = Randomf(-2.0f*PI, 2.0f*PI);
	element.dir.y = cosf(theta);
	element.dir.x = sinf(theta) * cosf(phi);
	element.dir.z = sinf(theta) * sinf(phi);
}

void ParticleDefaultBehavior::Update(ParticleElement & element)
{
	if (element.timer.TimeUp())
	{
		element.active = false;
		element.color.a = 0;
		return;
	}

	float progress = element.timer.Progress();

	element.transform.scale = this->timing_func["size"](this->start_size, this->end_size, progress) * Vector3::one;
	element.color = LerpC(this->start_color, this->end_color, progress);
	element.color.a = (UCHAR)(255*this->timing_func["opacity"](this->start_opacity, this->end_opacity, progress));
	element.transform.position += element.dir * this->start_speed /** powf(this->damping, element.timer.Elapsed())*/;

	element.timer.Step();
}

void ParticleDefaultBehavior::SetTimingFunc(std::string attr, std::function<float(float, float, float)> func)
{
	this->timing_func[attr] = func;
}

void IParticleBehavior::MakeElement(ParticleElement ** elements, UINT* pitch, UINT number)
{
	*elements = new ParticleElement[number];
	*pitch = sizeof(ParticleElement);
}
