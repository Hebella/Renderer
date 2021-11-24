#pragma once
#ifndef _SPHERE_H_
#define _SPHERE_H_
#include <vector>
#include "geometry.h"
#include "tgaimage.h"
class Sphere
{
private:
	vector<Vec3f> _vertex_coords; // 所有顶点的坐标
	vector<Vec2f> _texture_coords; // 纹理坐标
	vector<Vec3f> _n_coords; // 顶点法向量的坐标
	vector<Vec3<int>> _faces_vertex_idx; // 面对应的顶点坐标的编号
	vector<Vec3<int>> _faces_texture_idx; //面对应的纹理坐标的编号
	vector<Vec3<int>> _faces_n_idx; //面对应的法向量坐标的编号
public:
	Sphere();
	~Sphere();
	int nverts(); // 顶点个数
	int nfaces(); // 面个数
	Vec3f getVertexCoords(int i); // 获取编号为i的顶点坐标
	Vec2f getTextureCoords(int i); // 获取编号为i的顶点对应的纹理坐标
	Vec3f getNormalCoords(int i);
	Vec3<int> getFaceVertexId(int idx); // 获取编号为idx的面对应的顶点编号
	Vec3<int> getFaceTextureId(int idx); // 获取编号为idx的面上的顶点对应的纹理编号
	Vec3<int> getFaceNormalId(int idx);
};

#endif