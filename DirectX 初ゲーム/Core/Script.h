#pragma once

#include "Component.h"

class Script : public Component
{
public:
	Script(void);
	virtual void Init(void) {};
	virtual void Update(void) {};
	bool SetActive(bool value) override;
};