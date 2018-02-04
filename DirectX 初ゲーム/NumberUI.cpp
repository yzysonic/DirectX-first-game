#include "NumberUI.h"

NumberUI::NumberUI(int digits, int x, int y, std::string texture_digit, std::string texture_title)
{
	this->type = ObjectType::NumberUI;
	this->transform.position = Vector3((float)x, (float)y, 0.0f);

	if (texture_title != "none")
	{
		AddComponent<RectPolygon2D>(texture_title, Layer::UI_00);
		this->transform.position.x += Texture::Get(texture_title)->size.x / 2;
	}

	this->digitNum = digits;

	this->digitList.reserve(digits);
	for (int i = 0; i < digits; i++)
	{
		Object* digit = new Object;
		
		digit->AddComponent<RectPolygon2D>(texture_digit, Layer::UI_00);
		digit->transform.position.x;
		digit->transform.position.x = x + (digits - i - 0.5f)*Texture::Get(texture_digit)->size.x;
		digit->transform.position.y = (float)y;

		this->digitList.push_back(digit);
	}
}

NumberUI::~NumberUI()
{
	for (size_t i = 0; i < this->digitList.size(); i++)
		this->digitList[i]->Destroy();
	this->digitList.clear();

}

void NumberUI::setNumber(unsigned int value)
{
	this->value = value;
	for (int i = 0; i < this->digitNum; i++)
	{
		this->digitList[i]->GetComponent<RectPolygon2D>()->setPattern(value % 10);
		value /= 10;
	}
}

void NumberUI::setOffset(int x, int y)
{
	for (int i = 0; i < this->digitNum; i++)
	{
		this->digitList[i]->transform.position = this->transform.position + Vector3((float)x, (float)y, 0.0f);
		this->digitList[i]->transform.position.x += (this->digitNum - i - 0.5f)*this->digitList[i]->GetComponent<RectPolygon2D>()->getSize().x - GetComponent<RectPolygon2D>()->getSize().x/2;
	}
}

void NumberUI::SetColor(Color color)
{
	if(GetComponent<RectPolygon2D>() != nullptr)
		GetComponent<RectPolygon2D>()->SetColor(color);
	for (auto digit : digitList)
		digit->GetComponent<RectPolygon2D>()->SetColor(color);
}
