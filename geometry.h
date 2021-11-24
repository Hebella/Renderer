#pragma once
#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

template <class t> struct Vec2
{
	union {
		struct { t u, v; };
		struct { t x, y; };
		t raw[2];
	};
	Vec2() : u(0), v(0) {}
	Vec2(t _u, t _v) : u(_u), v(_v) {}
	inline Vec2<t> operator+(const Vec2<t>& V) const
	{
		return Vec2<t>(u + V.u, v + V.v);
	}
	inline Vec2<t> operator-(const Vec2<t>& V) const
	{
		return Vec2<t>(u - V.u, v - V.v);
	}
	inline Vec2<t> operator *(float f) const
	{
		return Vec2<t>(u * f, v * f);
	}
	inline t& operator[](int idx)
	{
		return raw[idx];
	}
	template <class > friend ostream& operator<<(ostream& s, Vec2<t>& v);
};

template <class t> struct Vec3
{
	union {
		struct { t x, y, z; };
		struct { t ivert, iuv, inorm; };
		t raw[3];
	};
	Vec3() : x(0), y(0), z(0) {}
	Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
	inline Vec3<t> operator+(const Vec3<t>& V) const
	{
		return Vec3<t>(x + V.x, y + V.y, z + V.z);
	}
	inline Vec3<t> operator-(const Vec3<t>& V) const
	{
		return Vec3<t>(x - V.x, y - V.y, z - V.z);
	}
	inline Vec3<t> operator*(float f) const
	{
		return Vec3<t>(x * f, y * f, z * f);
	}
	inline t operator*(const Vec3<t>& V) const
	{
		return (x * V.x + y * V.y + z * V.z);
	}
	inline Vec3<t> operator^(const Vec3<t>& v) const 
	{ 
		return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); 
	}
	inline t& operator[](int idx)
	{
		return raw[idx];
	}
	float norm() const 
	{ 
		return sqrt(x * x + y * y + z * z); 
	}
	Vec3<t>& normalize(t l = 1) 
	{ 
		*this = (*this) * (l / norm()); 
		return *this; 
	}

	template <class > friend ostream& operator<<(ostream& s, Vec3<t>& v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

template <class t> ostream& operator<<(ostream& s, Vec2<t>& v)
{
	s << "(" << v.x << ", " << v.y << ")\n";
	return s;
}

template <class t> ostream& operator<<(ostream& s, Vec3<t>& v)
{
	s << "(" << v.x << ", " << v.y << "," << v.z << ")\n";
	return s;
}

class Matrix
{
private:
	vector<vector<float>> _m;
	int _rows, _cols;
public:
	Matrix(int r = 4, int c = 4);
	inline int nrows();
	inline int nlines();

	static Matrix identity(int dims);
	vector<float>& operator[](int i);
	Matrix operator*(const Matrix& a);
	Matrix transpose();
	friend ostream& operator<<(ostream& s, Matrix& mat);
	Matrix inverse();
};



#endif