#include "RenderSpace.h"
#include "Renderer.h"

std::vector<smart_ptr<RenderSpace>> RenderSpace::render_space_list;
std::unordered_map<std::string, int> RenderSpace::name_map;


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
	catch (std::out_of_range&)
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
	catch (std::out_of_range&)
	{
		name_map.insert({name, render_space_list.size()});
		render_space_list.push_back(std::make_unique<RenderSpace>(name));
	}
}

void RenderSpace::Delete(int index)
{
	if(index < (int)render_space_list.size())
	{
		name_map.erase(render_space_list[index]->name);
		render_space_list[index].reset();
		render_space_list.erase(render_space_list.begin() + index);
	}
}

void RenderSpace::Delete(std::string name)
{
	try
	{
		render_space_list[name_map.at(name)].reset();
		render_space_list.erase(render_space_list.begin() + name_map.at(name));
		name_map.erase(name);
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
	for (auto &render_space:render_space_list)
	{
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
					drawable->draw();
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
