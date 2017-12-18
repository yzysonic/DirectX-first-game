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


MiniMap::MiniMap(int size_x, int size_y, int division_level)
{
	this->render_space = "mini_map";
	RenderSpace::Add(render_space);
	this->render_target = new RenderTarget(size_x, size_y);
	this->camera = new Camera(render_target);
	this->camera->setBackColor(255, 255, 255, 128);
	this->camera->fov = 0;
	Renderer::GetInstance()->setCamera(this->camera, render_space);
	this->division_level = division_level;
	this->zoom = 1.0f;
	this->is_shaking = false;
	this->timer = 0.0f;
	SetElement();
}

MiniMap::~MiniMap(void)
{
	for (auto &mark : enemy_list)
		delete mark;
	for (auto &element : element_list)
		delete element;

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

void MiniMap::SetPosition(Vector3 pos)
{
	this->transform->position = pos;
	Vector2 size = this->render_target->size / (float)division_level;
	
	for (int i = 0; i < (int)element_list.size(); i++)
	{
		auto &element = element_list[i];
		element->getTransform()->position.x = pos.x -0.5f*this->render_target->size.x + (i%division_level+0.5f)*size.x;
		element->getTransform()->position.y = pos.y +0.5f*this->render_target->size.y - (i / division_level+0.5f)*size.y;
		element->default_pos = element->getTransform()->position;
	}

}

void MiniMap::Shake(void)
{
	this->is_shaking = true;
}

void MiniMap::update(void)
{
	if (!this->player)
		return;

	this->camera->getTransform()->position.x = this->player->getTransform()->position.x;
	this->camera->getTransform()->position.y = this->player->getTransform()->position.y;

	if (is_shaking)
	{
		float progress = timer / 2.0f;

		for (int i = 0; i < (int)element_list.size(); i++)
		{
			auto &element = element_list[i];
			Vector3 shake_offset;

			// ƒp[ƒŠƒ“ƒmƒCƒY‚Å‹[Ž—U“®
			shake_offset.x = PerlinNoise(80.0f * (progress)+i*10);
			shake_offset.y = PerlinNoise(80.0f * (progress)+i*10 + 1);

			// ‹­“xŒ¸Š‚ÌŒvŽZ
			shake_offset *= exp2f(-7.f*progress) * 5.0f;

			element->getTransform()->position += shake_offset;

		}

		if (timer > 1.5f)
		{
			is_shaking = false;
			SetPosition(this->transform->position);
			timer = 0.0f;
		}

		timer += Time::DeltaTime();
	}
}

void MiniMap::SetElement(void)
{
	int element_count = division_level*division_level;

	this->element_list.reserve(element_count);

	Texture backup = *this->render_target;
	this->render_target->size /= (float)division_level;
	this->render_target->divideX =
	this->render_target->divideY = division_level;

	for (int i = 0; i < element_count; i++)
	{
		Element *element = new Element;
		element->setPolygon(Layer::UI_00, this->render_target, RendererType::UI);
		element->getPolygon()->setPattern(i);
		if (i % 2 == 0)
			element->getPolygon()->setColor(250, 250, 250, 255);
		element_list.push_back(element);
	}

	*dynamic_cast<Texture*>(this->render_target) = backup;

}

void MiniMap::Element::update(void)
{
	this->transform->position = Vector3::Lerp(this->transform->position, this->default_pos, Time::DeltaTime()*10.0f);
}
