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
	static void Draw(void);
	static RenderSpace* Get(int index);
	static RenderSpace* Get(std::string name);
	static void Add(std::string name);
	static void Add(std::string name, int order);
	static void Delete(int index);
	static void Delete(std::string name);
	static int RenderSpaceCount(void);

private:
	static std::vector<smart_ptr<RenderSpace>> render_space_list;
	static std::unordered_map<std::string, int> name_map;
	static std::vector<int> draw_order_map;

public:
	RenderSpace(std::string name);

	void AddDraw(Drawable* drawable);
	void RemoveDraw(Drawable* drawable);
	Camera* GetCamera(int no);
	void AddCamera(Camera* camera);
	void SetCamera(int no, Camera* camera);
	void RemoveCamera(Camera* camera);
	int CameraCount(void);
	int GetIndex(void);

private:
	std::string name;
	std::vector<Drawable*> draw_list[(int)Layer::MAX];
	std::vector<Camera*> camera_list;
};
