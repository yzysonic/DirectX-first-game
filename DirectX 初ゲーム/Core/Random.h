#pragma once
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <list>

void InitRandom(void);
int Random(int min, int max);
float Randomf(float min, float max);

class RandomTable
{
public:
	std::vector<float> prob_list;
	std::vector<std::size_t> alias_list;

	void Init(std::vector<float>& prob_list);
	int Next(void);
};
