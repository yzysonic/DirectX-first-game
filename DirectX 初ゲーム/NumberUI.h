#pragma once

#include "Core/Core.h"

class NumberUI : public Object2D
{
public:
	NumberUI(int digits, int x, int y, Texture* texture_digit, Texture* texture_title = Texture::none);
	~NumberUI(void);
	void setNumber(unsigned int value);
	void setOffset(int x, int y);
	void setColor(Color color);

protected:
	int digitNum;
	int value;
	std::vector<Object2D*> digitList;

};