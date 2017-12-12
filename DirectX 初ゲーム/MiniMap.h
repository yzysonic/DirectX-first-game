#pragma once
#include "Core/Core.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <string>


class MiniMap : public Object2D
{
public:
	class Mark : public Object2D
	{
	public:
		ObjectBase const *reference = nullptr;
		MiniMap const * parent = nullptr;

		Mark(MiniMap const * parent);
		void update(void) override;
	};

public:
	Camera* camera = nullptr;
	RenderTarget* render_target = nullptr;
	float zoom;

	MiniMap(int size_x, int size_y);
	~MiniMap(void);
	void SetPlayer(Player const* player);
	void SetEnemy(Enemy const* enemy);
	void RemoveEnemy(Enemy const* enemy);

	void update(void) override;

private:
	std::string render_space;
	Mark* player = nullptr;
	std::vector<Mark*> enemy_list;

};