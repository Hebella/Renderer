#pragma once
#ifndef _UTILS_H_
#define _UTILS_H_

#include "geometry.h"

Vec3f MatrixToPoint(Matrix m)
{
	return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

Matrix PointToMatrix(Vec3f v)
{

	Matrix m(4, 1);
	m[0][0] = v.x;
	m[1][0] = v.y;
	m[2][0] = v.z;
	m[3][0] = 1.0f;
	return m;
}

Matrix VecToMatrix(Vec3f n)
{
	Matrix m(4, 1);
	m[0][0] = n.x;
	m[1][0] = n.y;
	m[2][0] = n.z;
	m[3][0] = 0.0f;
	return m;
}

Vec3f MatrixToVec(Matrix m)
{
	return Vec3f(m[0][0], m[1][0], m[2][0]);
}

#endif