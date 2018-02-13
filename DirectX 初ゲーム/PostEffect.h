#pragma once
#include "Core\Core.h"

class PostEffect : public Object
{
public:
	PostEffect(void);
	~PostEffect(void);
	void SetActive(bool value) override;
	RenderTarget* GetInputRT(void);
	void SetBlur(float strength);
	void SetBlurActive(bool value);
	void SetCA(float interval);
	void Pause(void);
	void Resume(void);

private:
	RenderTarget* input;
	RenderTarget* output;
	RenderTarget* buffer;
	Camera* camera;
};