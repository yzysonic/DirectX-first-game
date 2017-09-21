#pragma once

#include "Core.h"

class NumberUI : public Object
{
public:
	NumberUI(int digits, int x, int y, TextureName texture_digit, TextureName texture_title = TEX_NONE);
	~NumberUI(void);
	void setNumber(unsigned int value);
	void setOffset(int x, int y);

private:
	int digitNum;
	int value;
	std::vector<Object*> digitList;

};