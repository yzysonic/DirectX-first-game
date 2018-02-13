#pragma once
#include "Direct3D.h"
#include <string>
#include <unordered_map>
#include <memory>

constexpr const char* ShaderPath = "Shader/";

class Shader
{
public:
	static void UnloadAll(void);

protected:
	static std::unordered_map<std::string, std::unique_ptr<Shader>> shader_list;

public:
	LPD3DXCONSTANTTABLE pConstantTable;

	~Shader(void);
	void SetBool(const char* name, bool value);
	void SetMatrix(const char* name, D3DXMATRIX& matrix);
	void SetFloat(const char* name, float value);
	void SetFloatArray(const char* name, const float* pf, UINT count);

private:
	D3DXHANDLE const_handle;

	void GetConstHandle(const char* name);
};

class VertexShader : public Shader
{
public:
	static VertexShader default_shader;
	static VertexShader* Get(std::string name);
	static void Load(std::string file_name);

public:
	LPDIRECT3DVERTEXSHADER9 pD3DShader;

	VertexShader(void);
	~VertexShader(void);
};

class PixelShader : public Shader
{
public:
	static PixelShader default_shader;
	static PixelShader* Get(std::string name);
	static void Load(std::string file_name);

public:
	LPDIRECT3DPIXELSHADER9 pD3DShader;

	PixelShader(void);
	~PixelShader(void);
};
