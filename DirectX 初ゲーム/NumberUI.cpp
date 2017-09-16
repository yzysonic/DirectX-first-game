#include "NumberUI.h"

NumberUI::NumberUI(int digits, int x, int y, TextureName texture_digit, TextureName texture_title)
{

	this->transform->position = Vector3((float)x, (float)y, 0.0f);

	if (texture_title != TEX_NONE)
	{
		this->setPolygon(Layer::UI_00, texture_title, RendererType::UI);
		this->transform->position.x += GetTexture(texture_title)->size.x / 2;
	}

	this->digitNum = digits;

	this->digitList.reserve(digits);
	for (int i = 0; i < digits; i++)
	{
		Object* digit = new Object;
		
		digit->setPolygon(Layer::UI_00, texture_digit, RendererType::UI);
		digit->getTransform()->position.x;
		digit->getTransform()->position.x = x + (digits - i - 0.5f)*GetTexture(texture_digit)->size.x;
		digit->getTransform()->position.y = (float)y;

		this->digitList.push_back(digit);
	}
}

NumberUI::~NumberUI()
{
	for (size_t i = 0; i < this->digitList.size(); i++)
		delete this->digitList[i];
	this->digitList.clear();

}

void NumberUI::setNumber(unsigned int value)
{
	this->value = value;
	for (int i = 0; i < this->digitNum; i++)
	{
		this->digitList[i]->getPolygon()->setPattern(value % 10);
		value /= 10;
	}
}

void NumberUI::setOffset(int x, int y)
{
	for (int i = 0; i < this->digitNum; i++)
	{
		this->digitList[i]->getTransform()->position = this->transform->position + Vector3((float)x, (float)y, 0.0f);
		this->digitList[i]->getTransform()->position.x += (this->digitNum - i - 0.5f)*this->digitList[i]->getPolygon()->getSize().x - this->polygon->getSize().x/2;
	}
}
