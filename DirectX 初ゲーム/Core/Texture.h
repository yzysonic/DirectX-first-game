#pragma once
#include "Direct3D.h"
#include "Vector.h"
#include <unordered_map>
#include <memory>
#include <string>

constexpr const char* TextureDir = "Data/Texture/";

class Texture
{
public:
	static void Init(void);
	static void Uninit(void);
	static Texture* Get(std::string name);
	static void LoadTexture(std::string name, std::string file_name = "", int divX = 1, int divY = 1);
	static void MakeTexture(std::string name, int width, int height);

	static Texture* const none;

public:
	std::string name;
	std::string file_name;
	LPDIRECT3DTEXTURE9 pDXTex;	// テクスチャポインタ
	Vector2 size;				// テクスチャサイズ
	int divideX;				// テクスチャ内X分割数
	int divideY;				// テクスチャ内Y分割数

	~Texture(void);

private:
	static std::unordered_map<std::string, std::unique_ptr<Texture>> texture_list;
};