#pragma once

class Color
{
public:
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
	//unsigned long data;

	Color(void);
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	//unsigned char getR(void);
	//unsigned char getG(void);
	//unsigned char getB(void);
	//unsigned char getA(void);
	
	void setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	//void setR(unsigned char value);
	//void setG(unsigned char value);
	//void setB(unsigned char value);
	//void setA(unsigned char value);

public:
	static const Color white;
	static const Color black;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color none;
};