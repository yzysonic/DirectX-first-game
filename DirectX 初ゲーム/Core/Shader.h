#pragma once
#include "Direct3D.h"
#include <string>
#include <unordered_map>
#include <memory>

constexpr const char* ShaderPath = "Shader/";

class VertexShader
{
public:
	static VertexShader default_shader;
	static VertexShader* Get(std::string name);
	static void Load(std::string file_name);

private:
	static std::unordered_map<std::string, std::unique_ptr<VertexShader>> shader_list;

public:
	LPDIRECT3DVERTEXSHADER9 pD3DShader;
	LPD3DXCONSTANTTABLE pConstantTable;

	VertexShader(void);
	~VertexShader(void);
};

class PixelShader
{
public:
	static PixelShader default_shader;
	static PixelShader* Get(std::string name);
	static void Load(std::string file_name);

private:
	static std::unordered_map<std::string, std::unique_ptr<PixelShader>> shader_list;

public:
	LPDIRECT3DPIXELSHADER9 pD3DShader;
	LPD3DXCONSTANTTABLE pConstantTable;

	PixelShader(void);
	~PixelShader(void);
};
