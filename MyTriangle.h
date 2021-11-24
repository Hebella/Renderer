#pragma once
#include "geometry.h"
#include "tgaimage.h"

class MyTriangle
{
public:
	static void triangle_1(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color);
	static void triangle_2(Vec3f *pts, TGAImage& image, TGAImage& texture, int width, int height, float** zbuffer, Vec2f* face_tex);
private:
	static Vec3f getBarycentricCoords(Vec3f P, Vec3f* pts);
};

