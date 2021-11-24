#pragma once
#include "tgaimage.h"
#include "geometry.h"
#include "FragmentShader.h"

class Rasterization
{
private:
	float** _zbuffer;
	float** _shadowbuffer;
	int _width;
	int _height;
	static Vec3f getBarycentricCoords(Vec3f P, Vec3f* pts);
public:
	Matrix screenToShadow;
	Rasterization(int width, int height);
	void triangle(Vec3f* vertex_coords, Vec2f* texture_coords, TGAImage& image, FragmentShader &fragShader, int mode, Vec3f* normal_coords, unsigned char* framebuffer, Vec3f eye);
	void clear_zbuffer();
};

