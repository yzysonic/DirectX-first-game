#pragma once

#include "Component.h"

class Script : public Component
{
public:
	Script(void);
	virtual void Init(void) {};
	virtual void Update(void) {};
	virtual void Uninit(void) {};
	bool SetActive(bool value) override;
};