#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "Drawable.h"
#include "Camera.h"
#include "Layer.h"

class RenderSpace
{
public:
	static RenderSpace* Get(int index);
	static RenderSpace* Get(std::string name);
	static void Add(std::string name);
	static void Delete(int index);
	static void Delete(std::string name);
	static int RenderSpaceCount(void);

private:
	static std::vector<RenderSpace*> render_space_list;
	static std::unordered_map<std::string, int> name_map;


public:
	RenderSpace(std::string name);

	static void Draw(void);
	void AddDraw(Drawable* drawable);
	void RemoveDraw(Drawable* drawable);
	Camera* GetCamera(int no);
	void AddCamera(Camera* camera);
	void RemoveCamera(Camera* camera);
	int CameraCount(void);

private:
	std::string name;
	std::vector<Drawable*> draw_list[(int)Layer::MAX];
	std::vector<Camera*> camera_list;
};
