#include "Random.h"
#include "Math.h"

void InitRandom(void)
{
	srand((unsigned)time(NULL));
}

int Random(int min, int max)
{
	int range = max - min;
	if (range <= 0)
		return min;
	else
		return (int)roundf(min + ( rand() * (float)range / RAND_MAX));
}

float Randomf(float min, float max)
{
	float range = max - min;
	if (range < 1.0e-35f)
		return min;
	else
		return (min + (rand() * range / RAND_MAX));
}

void RandomTable::Init(std::vector<float>& prob_list)
{
	//QlFhttp://www.keithschwarz.com/darts-dice-coins/

	float sum = 0.0f;

	for (auto p : prob_list)
		sum += p;

	std::list<std::size_t> small_list, large_list;
	for (std::size_t i = 0; i < prob_list.size(); i++)
	{
		auto& p = prob_list[i];

		p *= prob_list.size() / sum;

		if (p < 1.0f)
			small_list.push_back(i);
		else
			large_list.push_back(i);
	}

	this->prob_list.resize(prob_list.size());
	this->alias_list.resize(prob_list.size());

	while (!small_list.empty() && !large_list.empty())
	{
		std::size_t l = small_list.front();
		std::size_t g = large_list.front();

		small_list.pop_front();
		large_list.pop_front();

		this->prob_list[l] = prob_list[l];
		this->alias_list[l] = g;

		prob_list[g] = (prob_list[g] + prob_list[l]) - 1.0f;

		if (prob_list[g] < 1.0f)
			small_list.push_back(g);
		else
			large_list.push_back(g);
	}

	while (!large_list.empty())
	{
		this->prob_list[large_list.front()] = 1.0f;
		large_list.pop_front();
	}

	while (!small_list.empty())
	{
		this->prob_list[small_list.front()] = 1.0f;
		small_list.pop_front();
	}

}

int RandomTable::Next(void)
{
	if (this->prob_list.empty())
		return 0;

	float rand = Randomf(0.0f, this->prob_list.size() - 1.0f);

	// rand‚Ì®”•”
	int irand = (int)rand;

	// rand‚ÌÀ”•”
	float frand = rand - irand;

	if (frand < this->prob_list[irand])
		return irand;
	else
		return (int)this->alias_list[irand];
}
