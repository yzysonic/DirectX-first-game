#include "Shader.h"
#include "Common.h"
#include "Window.h"

std::unordered_map<std::string, std::unique_ptr<Shader>> Shader::shader_list;

VertexShader VertexShader::default_shader;
PixelShader PixelShader::default_shader;

VertexShader * VertexShader::Get(std::string name)
{
	try
	{
		return (VertexShader*)(shader_list.at(name).get());
	}
	catch (std::out_of_range)
	{
		return nullptr;
	}
}

void VertexShader::Load(std::string file_name)
{
	std::string name = file_name.substr(0, file_name.find_last_of("."));

	if (shader_list.find(name) != shader_list.end())
		return;

	auto pDevice = Direct3D::GetDevice();
	VertexShader* shader = new VertexShader;
	LPD3DXBUFFER pCode = nullptr;
	HRESULT hr;

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
	{
		MessageBox(Window::GetHWnd(), "バーテックスシェーダーのロード失敗。", _T("エラー"), MB_OK | MB_ICONWARNING);
		delete shader;
	}
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
}

PixelShader * PixelShader::Get(std::string name)
{
	try
	{
		return (PixelShader*)(shader_list.at(name).get());
	}
	catch (std::out_of_range)
	{
		return nullptr;
	}
}

void PixelShader::Load(std::string file_name)
{
	std::string name = file_name.substr(0, file_name.find_last_of("."));

	if (shader_list.find(name) != shader_list.end())
		return;

	auto pDevice = Direct3D::GetDevice();
	PixelShader* shader = new PixelShader;
	LPD3DXBUFFER pCode = nullptr;
#ifdef _DEBUG
	LPD3DXBUFFER pError = nullptr;
#endif
	HRESULT hr;

	hr = D3DXCompileShaderFromFile(
		(std::string(ShaderPath) + file_name).c_str(),
		NULL,
		NULL,
		name.c_str(),
		"ps_3_0",
		0,
		&pCode,
#ifdef _DEBUG
		&pError,
#else
		NULL,
#endif
		&shader->pConstantTable);

#ifdef _DEBUG
	if (FAILED(hr))
		OutputDebugString((char*)pError->GetBufferPointer());
#endif

	hr = pDevice->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), &shader->pD3DShader);

	SafeRelease(pCode);

	if (FAILED(hr))
	{
		MessageBox(Window::GetHWnd(), "ピクセルシェーダーのロード失敗。", _T("エラー"), MB_OK | MB_ICONWARNING);
		delete shader;
	}
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
}

void Shader::UnloadAll(void)
{
	shader_list.clear();
}

Shader::~Shader(void)
{
	SafeRelease(this->pConstantTable);
}

void Shader::SetBool(const char * name, bool value)
{
	GetConstHandle(name);
	this->pConstantTable->SetBool(Direct3D::GetDevice(), this->const_handle, value);
}

void Shader::SetMatrix(const char* name, D3DXMATRIX & matrix)
{
	GetConstHandle(name);
	this->pConstantTable->SetMatrix(Direct3D::GetDevice(), this->const_handle, &matrix);
}

void Shader::SetFloat(const char * name, float value)
{
	GetConstHandle(name);
	this->pConstantTable->SetFloat(Direct3D::GetDevice(), this->const_handle, value);
}

void Shader::SetFloatArray(const char * name, const float * pf, UINT count)
{
	GetConstHandle(name);
	this->pConstantTable->SetFloatArray(Direct3D::GetDevice(), this->const_handle, pf, count);
}

void Shader::GetConstHandle(const char * name)
{
	this->const_handle = this->pConstantTable->GetConstantByName(NULL, name);
}
