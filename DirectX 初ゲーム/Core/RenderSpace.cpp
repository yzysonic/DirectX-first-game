#include "RenderSpace.h"

std::vector<RenderSpace*> RenderSpace::render_space_list;
std::unordered_map<std::string, int> RenderSpace::name_map;


RenderSpace * RenderSpace::Get(int index)
{
	try
	{
		return render_space_list.at(index);
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
		return render_space_list[name_map.at(name)];
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
		render_space_list.push_back(new RenderSpace(name));
	}
}

void RenderSpace::Delete(int index)
{
	if(index < render_space_list.size())
	{
		name_map.erase(render_space_list[index]->name);
		delete render_space_list[index];
		render_space_list.erase(render_space_list.begin() + index);
	}
}

void RenderSpace::Delete(std::string name)
{
	try
	{
		delete render_space_list[name_map.at(name)];
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

	for (RenderSpace *render_space:render_space_list)
	{
		for (Camera* camera: render_space->camera_list)
		{
			pDevice->SetRenderTarget(0, camera->render_target->pSurface);
			pDevice->SetDepthStencilSurface(camera->render_target->pDepthSurface);

			// バックバッファ＆Ｚバッファのクリア
			pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), camera->getD3DBackColor(), 1.0f, 0);

			camera->getViewMatrix(true);
			camera->getProjectionMatrix(true);

			for (int i = 0; i < (int)Layer::MAX; i++)
			{
				auto list = render_space->draw_list[i];

				for (Drawable* drawable : list)
					drawable->draw();
			}

		}
	}

}

void RenderSpace::AddDraw(Drawable * drawable)
{
	drawable->list_index = this->draw_list[(int)drawable->layer].size();
	this->draw_list[(int)drawable->layer].push_back(drawable);
}

void RenderSpace::RemoveDraw(Drawable * drawable)
{
	int index = drawable->list_index;
	std::vector<Drawable*>& list = this->draw_list[(int)drawable->layer];

	list[index] = list.back();
	list[index]->list_index = index;
	list.pop_back();
}

Camera * RenderSpace::GetCamera(int no)
{
	if (no < camera_list.size())
		return camera_list[no];
	else
		return nullptr;
}

void RenderSpace::AddCamera(Camera * camera)
{
	camera_list.push_back(camera);
}

void RenderSpace::RemoveCamera(Camera * camera)
{
	for (int i = 0; i < camera_list.size(); i++)
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
