#pragma once

#include "Core/Core.h"

class NumberUI : public Object
{
public:
	NumberUI(int digits, int x, int y, std::string texture_digit, std::string texture_title = "none");
	~NumberUI(void);
	void setNumber(unsigned int value);
	void setOffset(int x, int y);
	void SetColor(Color color);

protected:
	int digitNum;
	int value;
	std::vector<Object*> digitList;

};