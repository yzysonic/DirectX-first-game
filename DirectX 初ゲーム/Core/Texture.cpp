#include "Texture.h"
#include "Common.h"
#include "Window.h"

Texture* const Texture::none = new Texture;
std::unordered_map<std::string, std::unique_ptr<Texture>> Texture::texture_list;

void Texture::Init(void)
{

	none->pDXTex = nullptr;
	none->name = "none";
	none->divideX = 1;
	none->divideY = 1;
	none->size = Vector2(100.0f, 100.0f);

	texture_list["none"].reset(none);
}

void Texture::Uninit(void)
{
	texture_list["none"].reset(nullptr);
	texture_list.erase("none");

	for (auto &tex : texture_list)
	{
		tex.second->pDXTex->Release();
		tex.second.reset(nullptr);
	}
	texture_list.clear();
}

Texture * Texture::Get(std::string name)
{
	try
	{
		return texture_list.at(name).get();
	}
	catch (std::out_of_range&)
	{
		return none;
	}
}

void Texture::LoadTexture(std::string name, std::string file_name, int divX, int divY)
{

	if (texture_list[name])
		return;

	char file_dir[256];

	if (file_name == "")
		file_name = name + ".png";
	strcat(strcpy(file_dir, TextureDir), file_name.c_str());

	Texture *texture = new Texture;

	//テクスチャ詳細情報取得
	D3DXIMAGE_INFO info;
	if (D3DXGetImageInfoFromFile(file_dir, &info) == D3D_OK)
	{
		//テクスチャ読込
		D3DXCreateTextureFromFileEx(
			Direct3D::GetDevice(),
			file_dir,
			info.Width, info.Height,
			1, 0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_FILTER_NONE,
			0x00000000,
			NULL, NULL,
			&texture->pDXTex);
	}
	else
	{
		TCHAR s[128];
		wsprintf(s, _T("テクスチャー「%s」の読込に失敗しました。"), file_name.c_str());
		MessageBox(Window::GetHWnd(), s, _T("エラー"), MB_OK | MB_ICONWARNING);

		delete texture;

		return;

	}

	texture->name = name;
	texture->file_name = file_name;
	texture->size.x = (float)info.Width / divX;
	texture->size.y = (float)info.Height / divY;
	texture->divideX = divX;
	texture->divideY = divY;

	texture_list[name].reset(texture);

}
