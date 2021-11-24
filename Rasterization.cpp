#include "Rasterization.h"
#include "tgaimage.h"
#include "Transform.h"

Rasterization::Rasterization(int width, int height)
{
	_width = width;
	_height = height;
	_zbuffer = new float * [width];
	_shadowbuffer = new float* [width];
	for (int i = 0; i < width; ++i)
	{
		_zbuffer[i] = new float[height];
		_shadowbuffer[i] = new float[height];
		for (int j = 0; j < height; ++j)
		{
			_zbuffer[i][j] = numeric_limits<float>::min();
			_shadowbuffer[i][j] = numeric_limits<float>::min();
		}
	}
}


void Rasterization::triangle(Vec3f* vertex_coords, Vec2f* texture_coords, TGAImage& image, FragmentShader &fragShader, int mode, Vec3f* normal_coords, unsigned char* framebuffer, Vec3f eye)
{
	Vec2f box_min(_width - 1, _height - 1), box_max(0, 0), upper_bound(_width - 1, _height - 1);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			box_min[j] = max(0.0f, min(box_min[j], vertex_coords[i][j]));
			box_max[j] = min(upper_bound[j], max(box_max[j], vertex_coords[i][j]));
		}
	}

	float subsample[4][2] =
	{
		{-0.25,-0.25},
		{ 0.25,-0.25},
		{-0.25, 0.25},
		{ 0.25, 0.25},
	};

	Vec3f P;
	for (P.x = box_min[0]; P.x <= box_max[0]; ++P.x)
	{
		for (P.y = box_min[1]; P.y <= box_max[1]; ++P.y)
		{
			int count = 0;

			/*for (int i = 0; i < 4; i++) // 抗锯齿化
			{
				Vec3f sub_P;
				sub_P.x = P.x + subsample[i][0];
				sub_P.y = P.y + subsample[i][1];

				Vec3f sub_barycentric_coords = getBarycentricCoords(sub_P, vertex_coords);
				if (sub_barycentric_coords.x < 0 || sub_barycentric_coords.y < 0 || sub_barycentric_coords.z < 0)
					continue;

				count += 1;
			}*/

			Vec3f barycentric_coords = Rasterization::getBarycentricCoords(P, vertex_coords);
			if (barycentric_coords.x < -0.01 || barycentric_coords.y < -0.01 || barycentric_coords.z < -0.01)
				continue;
			P.z = barycentric_coords.z * vertex_coords[0].z + barycentric_coords.x * vertex_coords[1].z + barycentric_coords.y * vertex_coords[2].z;
			switch (mode)
			{
			case(0):
				_shadowbuffer[int(P.x)][int(P.y)] =  max(_shadowbuffer[int(P.x)][int(P.y)], float(P.z));
				break;
			case(1):
				if (_zbuffer[int(P.x)][int(P.y)] < P.z) // P在靠近摄像机的地方
				{
					_zbuffer[int(P.x)][int(P.y)] = P.z;
					//Vec3f shadow_coords = Transform::MatrixToVec(screenToShadow * Transform::VecToMatrix(Vec3f(P.x, P.y, P.z)));
					float shadow = count / 4.0; // .7 + .3 * (_shadowbuffer[int(shadow_coords.x)][int(shadow_coords.y)] < shadow_coords.z); //在阴影里
					fragShader.Render(P, barycentric_coords, texture_coords, image, shadow, normal_coords, framebuffer, eye);
				}
				break;
			default:
				break;
			}
		}
	}
}

void Rasterization::clear_zbuffer()
{
	for (int i = 0; i < _width; ++i)
	{
		for (int j = 0; j < _height; ++j)
		{
			_zbuffer[i][j] = numeric_limits<float>::min();
		}
	}
}

Vec3f Rasterization::getBarycentricCoords(Vec3f P, Vec3f* vertex_coords)
{
	Vec3f u = Vec3f(vertex_coords[1].x - vertex_coords[0].x, vertex_coords[2].x - vertex_coords[0].x, vertex_coords[0].x - P.x) ^ Vec3f(vertex_coords[1].y - vertex_coords[0].y, vertex_coords[2].y - vertex_coords[0].y, vertex_coords[0].y - P.y); // (AB, AC, PA)
	if (abs(u.z) < 1) // 整数向量的叉乘也是整数向量
		return Vec3f(-1, 1, 1);
	return Vec3f(u.x / u.z, u.y / u.z, 1.0f - u.x / u.z - u.y / u.z); // 对应(u, v, 1 - u - v)
}
