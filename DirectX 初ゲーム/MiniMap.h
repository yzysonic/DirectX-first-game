#pragma once
#include "Core/Core.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <string>


class MiniMap : public Object
{
private:
	class Mark : public Object
	{
	public:
		Object const *reference = nullptr;
		MiniMap const * parent = nullptr;

		Mark(MiniMap const * parent);
		void Update(void) override;
	};
	class Element : public Object
	{
	public:
		Vector3 default_pos;
		void Update(void) override;
	};

public:
	Camera* camera = nullptr;
	RenderTarget* render_target = nullptr;
	float zoom;

	MiniMap(int size_x, int size_y, int division_level);
	~MiniMap(void);
	void SetPlayer(Player const* player);
	void SetEnemy(Enemy const* enemy);
	void RemoveEnemy(Enemy const* enemy);
	void SetPosition(Vector3 pos);
	void Shake(void);

	void Update(void) override;

private:
	int division_level;
	bool is_shaking;
	float timer;
	std::string render_space;
	Mark* player = nullptr;
	std::vector<Mark*> enemy_list;
	std::vector<Element*> element_list;

	void SetElement(void);

};