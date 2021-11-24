#pragma once
#include "tgaimage.h"
#include "geometry.h"
#include "FragmentData.h"
#include <string>

using namespace std;

class FragmentShader
{
	TGAImage *texture;
	TGAImage* faces[6];
	int _width;
	int _height;
	int _alpha;
	Vec3f _light;
	bool _isSkyBox;
public:
	FragmentShader(int width, int height, const string filename, bool isSkyBox);
	FragmentShader(int width, int height, TGAColor c);
	void Render(Vec3f P, Vec3f barycentric_coords, Vec2f* texture_coords, Vec3f* normal_coords, unsigned char* framebuffer, FragmentData*** zbuffer, Vec3f* clipcoord_attri);
};

