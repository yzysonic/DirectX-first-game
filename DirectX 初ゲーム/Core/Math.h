#pragma once

#include "Easing.h"
#include "Lerp.h"
#include "PerlinNoise.h"
#include "Random.h"

constexpr float PI = 3.141592654f;
constexpr float Deg2Rad(float degree)
{
	return degree / 180.0f*PI;
}

constexpr float Rad2Deg(float radian)
{
	return radian / PI*180.0f;
}