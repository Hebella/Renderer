#include "Sphere.h"
#define PI 3.1415926

Sphere::Sphere()
{
	int M = 20, N = 40; // M是纵向细分程度，N是横向细分程度
	int vertNum = M * N * 4;//顶点总数

	float stepAngZ = PI / M;//纵向角度每次增加的值
	float stepAngXY = PI * 2 / N;//横向角度每次增加的值
	float angZ = 0.0;//初始的纵向角度
	float angXY = 0.0;//初始的横向角度

	int index = 0;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			//构造一个顶点
			float x1 = sin(angZ) * cos(angXY);
			float y1 = sin(angZ) * sin(angXY);
			float z1 = cos(angZ);
			_vertex_coords.push_back(Vec3f(x1, y1, z1));
			_n_coords.push_back(Vec3f(x1, y1, z1)); // 球体的法向量就是顶点坐标

			float x2 = sin(angZ + stepAngZ) * cos(angXY);
			float y2 = sin(angZ + stepAngZ) * sin(angXY);
			float z2 = cos(angZ + stepAngZ);
			_vertex_coords.push_back(Vec3f(x2, y2, z2));
			_n_coords.push_back(Vec3f(x2, y2, z2));

			float x3 = sin(angZ + stepAngZ) * cos(angXY + stepAngXY);
			float y3 = sin(angZ + stepAngZ) * sin(angXY + stepAngXY);
			float z3 = cos(angZ + stepAngZ);
			_vertex_coords.push_back(Vec3f(x3, y3, z3));
			_n_coords.push_back(Vec3f(x3, y3, z3));

			float x4 = sin(angZ) * cos(angXY + stepAngXY);
			float y4 = sin(angZ) * sin(angXY + stepAngXY);
			float z4 = cos(angZ);
			_vertex_coords.push_back(Vec3f(x4, y4, z4));
			_n_coords.push_back(Vec3f(x4, y4, z4));

			_faces_vertex_idx.push_back(Vec3<int>(index, index + 3, index + 2)); // 4 1
			_faces_vertex_idx.push_back(Vec3<int>(index, index + 2, index + 1)); // 3 2
			_faces_n_idx.push_back(Vec3<int>(index, index + 3, index + 2)); // 4 1
			_faces_n_idx.push_back(Vec3<int>(index, index + 2, index + 1)); // 3 2

			angXY += stepAngXY;
			index += 4;
		}
		angXY = 0.0;//每次横向到达2PI角度则横向角度归0
		angZ += stepAngZ;
	}

	_texture_coords.push_back(Vec2f(0.0f, 0.0f));
	_faces_texture_idx.push_back(Vec3i(0, 0, 0));

	cout << vertNum << " " << _vertex_coords.size() << endl;
}

Sphere::~Sphere()
{

}

int Sphere::nverts()
{
	return (int)_vertex_coords.size();
}

int Sphere::nfaces()
{
	return (int)_faces_vertex_idx.size();
}

Vec3<int> Sphere::getFaceVertexId(int idx)
{
	return _faces_vertex_idx[idx];
}

Vec3<int> Sphere::getFaceTextureId(int idx)
{
	return _faces_texture_idx[0];
}

Vec3<int> Sphere::getFaceNormalId(int idx)
{
	return _faces_n_idx[idx];
}

Vec3f Sphere::getVertexCoords(int i)
{
	return _vertex_coords[i];
}

Vec2f Sphere::getTextureCoords(int i)
{
	return _texture_coords[0];
}

Vec3f Sphere::getNormalCoords(int i)
{
	return _n_coords[i];
}