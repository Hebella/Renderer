#pragma once
#include "tgaimage.h"
#include <limits>

struct FragmentData
{
	float _depth;
	TGAColor _color;
	FragmentData* _next;
	FragmentData(TGAColor color = TGAColor(255, 255, 255, 255), float depth = -10000.0f, FragmentData* next = NULL)
	{
		_color = color;
		_depth = depth;
		_next = next;
	}
};