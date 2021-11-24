#pragma once
#include "geometry.h"
class Transform
{
public:
	static Vec3f MatrixToVec(Matrix m);
	static Matrix VecToMatrix(Vec3f v);
	static Matrix ViewToScreen(int left, int top, int w, int h, int depth);
	static Matrix LookAt(Vec3f eye, Vec3f center, Vec3f up);
	static Matrix Projection(Vec3f camera);
	static Matrix Projection(float r, float t, float n, float f);
	static Matrix Scale(float sx, float sy, float sz);
	static Matrix Translate(float tx, float ty, float tz);
};

