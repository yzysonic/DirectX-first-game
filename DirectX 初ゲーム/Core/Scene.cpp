#include "Scene.h"
#include "ObjectManager.h"

Scene::~Scene(void)
{
	for (auto obj : this->obj_list)
	{
		obj->scene = nullptr;
		obj->Destroy();
	}

	this->obj_list.clear();

	ObjectManager::KillObject();
}

void Scene::RemoveObject(Object * object)
{
	this->obj_list[object->sceneIndex] = this->obj_list.back();
	this->obj_list[object->sceneIndex]->sceneIndex = object->sceneIndex;
	this->obj_list.pop_back();
}


void Scene::UpdateObjects(void)
{
	for (UINT i = 0; i < this->obj_list.size(); i++)
	{
		auto object = this->obj_list[i];
		if (object->isActive)
		{
			// �I�u�W�F�N�g�̍X�V����
			object->Update();

			// �I�u�W�F�N�g�����̃X�N���v�g�̍X�V����
			for (UINT j = 0; j < object->scripts.size(); j++)
			{
				auto &script = object->scripts[j];
				if (script->GetActive())
					script->Update();
			}
		}
	}

}
