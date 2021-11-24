#pragma once
#include "tgaimage.h"
#include "Transform.h"
#include "FragmentShader.h"
#include <vector>

struct Plane
{
	Vec3f P;
	Vec3f N;
	Plane(Vec3f p, Vec3f n) : P(p), N(n) {}
};

class VertexShader
{
private:
	int WIDTH, HEIGHT;
	FragmentData*** _zbuffer;
	FragmentShader* _fragShader;
	unsigned char* _framebuffer;
	Matrix _projMat;
	Matrix _modelMat;
	Matrix _viewScreenMat;
	float _proj_right;
	float _proj_top;
	float _proj_near;
	float _proj_far;

	void triangle(Vec3f* vertex_coords, Vec2f* texture_coords, Vec3f* normal_coords, Vec3f* clipcoord_attri);
	Vec3f getBarycentricCoords(Vec3f P, Vec3f* pts);
	static Vec3f MatrixToPoint(Matrix m);
	static Matrix PointToMatrix(Vec3f v);
	static Matrix VecToMatrix(Vec3f n);
	Vec3f MatrixToVec(Matrix m);
	vector<Vec3f> clip_with_plane(Plane clipping_plane, const vector<Vec3f>& vertex_coords, vector<Vec2f>& texture_coords);

public:
	VertexShader(int width, int height, FragmentData*** zbuffer, FragmentShader* fragShader, unsigned char* framebuffer, Matrix projMat, Matrix modelMat, Matrix viewScreenMat, float proj_right, float proj_top, float proj_near, float proj_far)
	{
		WIDTH = width;
		HEIGHT = height;
		_zbuffer = zbuffer;
		_fragShader = fragShader;
		_framebuffer = framebuffer;
		_projMat = projMat;
		_modelMat = modelMat;
		_viewScreenMat = viewScreenMat;
		_proj_right = proj_right;
		_proj_top = proj_top;
		_proj_near = proj_near;
		_proj_far = proj_far;
	}
	void Render(Vec3f* vertex_coords, Vec2f* texture_coords, Vec3f* normal_coords, Matrix viewMat);
};

