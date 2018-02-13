#include "PostEffect.h"
#include "GaussianBlur.h"
#include "ChromaticAberration.h"
#include "ChromaticAberrationScript.h"

PostEffect::PostEffect(void)
{
	// レンダースペースの追加
	RenderSpace::Add("post");

	// カメラ初期化
	this->camera = new Camera;
	RenderSpace::Get("post")->AddCamera(this->camera);

	// レンダーターゲット初期化
	this->input = new RenderTarget();
	this->output = new RenderTarget();
	this->buffer = new RenderTarget();

	AddComponent<RectPolygon2D>(this->output, Layer::TOP, "post");

	AddComponent<GaussianBlur>(this->input, this->buffer);
	AddComponent<ChromaticAberration>(this->buffer, this->output);

	AddComponent<ChromaticAberrationScript>();

	SetBlurActive(false);
}

PostEffect::~PostEffect(void)
{
	SafeDelete(this->input);
	SafeDelete(this->output);
	SafeDelete(this->buffer);
}

void PostEffect::SetActive(bool value)
{
	if (this->isActive == value)
		return;

	Object::SetActive(value);

	if (value)
	{
		RenderSpace::Get("post")->AddCamera(this->camera);
	}
	else
	{
		RenderSpace::Get("post")->RemoveCamera(this->camera);
	}
}

RenderTarget * PostEffect::GetInputRT(void)
{
	return this->input;
}

void PostEffect::SetBlur(float strength)
{
	if(strength >= 0.0f)
		GetComponent<GaussianBlur>()->strength = 1.0f+strength;
}

void PostEffect::SetBlurActive(bool value)
{
	auto blur = GetComponent<GaussianBlur>();

	if (!blur->SetActive(value))
		return;

	if (value)
	{
		GetComponent<ChromaticAberration>()->SetTexture(this->buffer);
	}
	else
	{
		GetComponent<ChromaticAberration>()->SetTexture(this->input);
	}
}

void PostEffect::SetCA(float interval)
{
	GetComponent<ChromaticAberrationScript>()->interval = interval;
	GetComponent<ChromaticAberrationScript>()->SetActive(true);
}

void PostEffect::Pause(void)
{
	GetComponent<ChromaticAberrationScript>()->pause = true;
}

void PostEffect::Resume(void)
{
	GetComponent<ChromaticAberrationScript>()->pause = false;
}
