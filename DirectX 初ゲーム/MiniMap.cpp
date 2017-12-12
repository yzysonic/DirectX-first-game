#include "MiniMap.h"

MiniMap::Mark::Mark(MiniMap const * parent)
{
	this->parent = parent;
	this->setPolygon(Layer::DEFAULT, Texture::none, RendererType::Classic2D, parent->render_space);
	this->getTransform()->scale = 0.3f * Vector3::one;
}

void MiniMap::Mark::update(void)
{
	this->transform->position.x = this->reference->getTransform()->position.x / SystemParameters::ResolutionX * this->parent->render_target->size.x;
	this->transform->position.y = this->reference->getTransform()->position.y / SystemParameters::ResolutionY * this->parent->render_target->size.y;
	this->transform->position *= this->parent->zoom;
	this->transform->setRotation(this->reference->getTransform()->getRotation());
}


MiniMap::MiniMap(int size_x, int size_y)
{
	this->render_space = "mini_map";
	RenderSpace::Add(render_space);
	this->render_target = new RenderTarget(size_x, size_y);
	this->camera = new Camera(render_target);
	this->camera->setBackColor(255, 255, 255, 128);
	this->camera->fov = 0;
	Renderer::GetInstance()->setCamera(this->camera, render_space);
	this->setPolygon(Layer::UI_00, render_target, RendererType::UI);
	this->zoom = 1.0f;
}

MiniMap::~MiniMap(void)
{
	for (auto &mark : enemy_list)
		delete mark;

	SafeDelete<Mark>(this->player);

	delete this->camera;
	delete this->render_target;
	RenderSpace::Delete("mini_map");
}

void MiniMap::SetPlayer(Player const * player)
{
	if (this->player == nullptr)
		this->player = new Mark(this);

	this->player->reference = player;
	this->player->getPolygon()->setTexture(Texture::Get("player_mark"));
}

void MiniMap::SetEnemy(Enemy const * enemy)
{

	for (auto &mark : enemy_list)
	{
		if (mark->reference == nullptr)
		{
			mark->reference = enemy;
			mark->setActive(true);
			mark->getPolygon()->setTexture(Texture::Get("enemy_mark"));
			return;
		}
	}

	Mark* mark = new Mark(this);
	mark->reference = enemy;
	mark->getPolygon()->setTexture(Texture::Get("enemy_mark"));
	this->enemy_list.push_back(mark);

}

void MiniMap::RemoveEnemy(Enemy const * enemy)
{
	for (auto &mark : enemy_list)
	{
		if (mark->reference == enemy)
		{
			mark->reference = nullptr;
			mark->setActive(false);
			return;
		}
	}
	
}

void MiniMap::update(void)
{
	this->camera->getTransform()->position.x = this->player->getTransform()->position.x;
	this->camera->getTransform()->position.y = this->player->getTransform()->position.y;
}

