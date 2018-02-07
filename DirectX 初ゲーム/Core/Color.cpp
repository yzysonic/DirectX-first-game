#include "Color.h"

const Color Color::white	= Color(255, 255, 255, 255);
const Color Color::black	= Color(0, 0, 0, 255);
const Color Color::red		= Color(255, 0, 0, 255);
const Color Color::green	= Color(0, 255, 0, 255);
const Color Color::blue		= Color(0, 0, 255, 255);
const Color Color::none		= Color(0, 0, 0, 0);


Color::Color(void) : Color(0,0,0,0)
{
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	setRGBA(r, g, b, a);
}

//unsigned char Color::getR(void)
//{
//	return (unsigned char)(this->data >> 16);
//}
//
//unsigned char Color::getG(void)
//{
//	return (unsigned char)(this->data >> 8);
//}
//
//unsigned char Color::getB(void)
//{
//	return (unsigned char)(this->data);
//}
//
//unsigned char Color::getA(void)
//{
//	return (unsigned char)(this->data >> 24);
//}

void Color::setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	//this->data = (unsigned long)((a << 24) | (r << 16) | (g << 8) | (b));
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

//void Color::setR(unsigned char value)
//{
//	this->data &= 0xff00ffff;
//	this->data += (unsigned long)value << 16;
//}
//
//void Color::setG(unsigned char value)
//{
//	this->data &= 0xffff00ff;
//	this->data += (unsigned long)value << 8;
//}
//
//void Color::setB(unsigned char value)
//{
//	this->data &= 0xffffff00;
//	this->data += value;
//}
//
//void Color::setA(unsigned char value)
//{
//	this->data &= 0x00ffffff;
//	this->data += (unsigned long)value << 24;
//}
