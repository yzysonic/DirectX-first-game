#include "Shader.h"
#include "Common.h"
#include "Window.h"

std::unordered_map<std::string, std::unique_ptr<VertexShader>> VertexShader::shader_list;
std::unordered_map<std::string, std::unique_ptr<PixelShader>> PixelShader::shader_list;
VertexShader VertexShader::default_shader;
PixelShader PixelShader::default_shader;

VertexShader * VertexShader::Get(std::string name)
{
	try 
	{
		return shader_list.at(name).get();
	}
	catch (std::out_of_range)
	{
		return nullptr;
	}
}

void VertexShader::Load(std::string file_name)
{
	auto pDevice = Direct3D::GetDevice();
	VertexShader* shader = new VertexShader;
	LPD3DXBUFFER pCode = nullptr;
	HRESULT hr;
	std::string name = file_name.substr(0, file_name.find_last_of("."));

	hr =  D3DXCompileShaderFromFile(
		(std::string(ShaderPath) + file_name).c_str(),
		NULL,
		NULL,
		name.c_str(),
		"vs_2_0",
		0,
		&pCode,
		NULL,
		&shader->pConstantTable);

	hr = pDevice->CreateVertexShader((DWORD*)pCode->GetBufferPointer(), &shader->pD3DShader);

	SafeRelease(pCode);

	if (FAILED(hr))
		MessageBox(Window::GetHWnd(), "バーテックスシェーダーのロード失敗。", _T("エラー"), MB_OK | MB_ICONWARNING);
	else
		shader_list[name].reset(shader);
}

VertexShader::VertexShader(void)
{
	this->pD3DShader = nullptr;
	this->pConstantTable = nullptr;
}

VertexShader::~VertexShader(void)
{
	SafeRelease(this->pD3DShader);
	SafeRelease(this->pConstantTable);
}

PixelShader * PixelShader::Get(std::string name)
{
	return nullptr;
}

void PixelShader::Load(std::string file_name)
{
	auto pDevice = Direct3D::GetDevice();
	PixelShader* shader = nullptr;
	LPD3DXBUFFER pCode = nullptr;
	HRESULT hr;
	std::string name = file_name.substr(0, file_name.find_last_of("."));

	hr = D3DXCompileShaderFromFile(
		(std::string(ShaderPath) + file_name).c_str(),
		NULL,
		NULL,
		name.c_str(),
		"vs_2_0",
		0,
		&pCode,
		NULL,
		&shader->pConstantTable);

	hr = pDevice->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), &shader->pD3DShader);

	SafeRelease(pCode);

	if (FAILED(hr))
		MessageBox(Window::GetHWnd(), "ピクセルシェーダーのロード失敗。", _T("エラー"), MB_OK | MB_ICONWARNING);
	else
		shader_list[name].reset(shader);
}

PixelShader::PixelShader(void)
{
	this->pD3DShader = nullptr;
	this->pConstantTable = nullptr;
}

PixelShader::~PixelShader(void)
{
	SafeRelease(this->pD3DShader);
	SafeRelease(this->pConstantTable);
}

