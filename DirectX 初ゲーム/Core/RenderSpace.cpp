#include "RenderSpace.h"
#include "Renderer.h"

std::vector<smart_ptr<RenderSpace>> RenderSpace::render_space_list;
std::unordered_map<std::string, int> RenderSpace::name_map;
std::vector<int> RenderSpace::draw_order_map;


RenderSpace * RenderSpace::Get(int index)
{
	try
	{
		return render_space_list.at(index).get();
	}
	catch (std::out_of_range)
	{
		return nullptr;
	}
	
}

RenderSpace * RenderSpace::Get(std::string name)
{
	try
	{
		return render_space_list[name_map.at(name)].get();
	}
	catch (std::out_of_range)
	{
		return nullptr;
	}
}

void RenderSpace::Add(std::string name)
{
	try 
	{
		name_map.at(name);
	}
	catch (std::out_of_range)
	{
		name_map.insert({name, render_space_list.size()});
		render_space_list.push_back(std::make_unique<RenderSpace>(name));
		draw_order_map.emplace_back(name_map[name]);
	}
}

void RenderSpace::Add(std::string name, int order)
{
	Add(name);
	if (order < (int)draw_order_map.size() - 1)
	{
		draw_order_map.insert(draw_order_map.begin() + order, draw_order_map.back());
		draw_order_map.pop_back();
	}
}

void RenderSpace::Delete(int index)
{
	if(index < (int)render_space_list.size())
	{
		render_space_list[index].swap(render_space_list.back());
		name_map[render_space_list[index]->name] = index;
		name_map.erase(render_space_list.back()->name);
		render_space_list.pop_back();

		for (auto it = draw_order_map.begin(); it != draw_order_map.end();)
		{
			if (*it == index)
			{
				draw_order_map.erase(it);
				break;
			}
			else
				it++;
		}
		for (auto& i : draw_order_map)
		{
			if (i > index)
				i--;
		}
	}
}

void RenderSpace::Delete(std::string name)
{
	try
	{
		Delete(name_map.at(name));
	}
	catch (std::out_of_range){}
}

int RenderSpace::RenderSpaceCount(void)
{
	return render_space_list.size();
}

RenderSpace::RenderSpace(std::string name)
{
	this->name = name;
}

void RenderSpace::Draw(void)
{
	auto pDevice = Direct3D::GetDevice();

	// 描画空間ごとに
	for (auto &index:draw_order_map)
	{
		auto &render_space = render_space_list[index];

		// カメラごとに
		for (Camera* camera: render_space->camera_list)
		{
			Renderer::GetInstance()->drawing_camera = camera;

			pDevice->SetRenderTarget(0, camera->render_target->pSurface);
			pDevice->SetDepthStencilSurface(camera->render_target->pDepthSurface);

			// バックバッファ＆Ｚバッファのクリア
			pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), camera->backColor, 1.0f, 0);

			// ヴューマトリクス、プロジェクションマトリクスの計算
			camera->getViewMatrix(true);
			camera->getProjectionMatrix(true);

			// レイヤーごとに
			for (int i = 0; i < (int)Layer::MAX; i++)
			{
				// 描画リストの取得
				auto list = render_space->draw_list[i];

				for (Drawable* drawable : list)
				{
					drawable->object->OnDraw();
					drawable->Draw();
					drawable->object->AfterDraw();
				}
			}

		}
	}

}

//=============================================================================
// 描画リストに追加
//=============================================================================
void RenderSpace::AddDraw(Drawable * drawable)
{
	drawable->list_index = this->draw_list[(int)drawable->layer].size();
	drawable->render_space_index = this->GetIndex();
	this->draw_list[(int)drawable->layer].push_back(drawable);
}

//=============================================================================
// 描画リストから削除
//=============================================================================
void RenderSpace::RemoveDraw(Drawable * drawable)
{
	int &index = drawable->list_index;
	if (index < 0)
		return;

	auto& list = this->draw_list[(int)drawable->layer];

	list[index] = list.back();
	list[index]->list_index = index;
	list.pop_back();
	index = -1;
	drawable->render_space_index = -1;
}

Camera * RenderSpace::GetCamera(int no)
{
	if (no < (int)camera_list.size())
		return camera_list[no];
	else
		return nullptr;
}

void RenderSpace::AddCamera(Camera * camera)
{
	camera_list.push_back(camera);
}

void RenderSpace::SetCamera(int no, Camera * camera)
{
	camera_list[no] = camera;
}

void RenderSpace::RemoveCamera(Camera * camera)
{
	for (int i = 0; i < (int)camera_list.size(); i++)
	{
		if (camera_list[i] == camera)
		{
			camera_list.erase(camera_list.begin() + i);
			return;
		}
	}
}

int RenderSpace::CameraCount(void)
{
	return camera_list.size();
}

int RenderSpace::GetIndex(void)
{
	return name_map[this->name];
}
