#pragma once
#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <vector>
#include "geometry.h"
#include "tgaimage.h"
#include "FragmentShader.h"
#include "VertexShader.h"

class Object
{
protected:
	vector<Vec3f> _vertex_coords; // 所有顶点的坐标
	vector<Vec2f> _texture_coords; // 纹理坐标
	vector<Vec3f> _n_coords; // 顶点法向量的坐标
	vector<Vec3<int>> _faces_vertex_idx; // 面对应的顶点坐标的编号
	vector<Vec3<int>> _faces_texture_idx; //面对应的纹理坐标的编号
	vector<Vec3<int>> _faces_n_idx; //面对应的法向量坐标的编号
public:
	virtual ~Object();
	virtual int nverts() = 0; // 顶点个数
	virtual int nfaces() = 0; // 面个数
	virtual Vec3f getVertexCoords(int i) = 0; // 获取编号为i的顶点坐标
	virtual Vec2f getTextureCoords(int i) = 0; // 获取编号为i的顶点对应的纹理坐标
	virtual Vec3f getNormalCoords(int i) = 0;
	virtual Vec3<int> getFaceVertexId(int idx) = 0; // 获取编号为idx的面对应的顶点编号
	virtual Vec3<int> getFaceTextureId(int idx) = 0; // 获取编号为idx的面上的顶点对应的纹理编号
	virtual Vec3<int> getFaceNormalId(int idx) = 0;
	virtual void draw(Matrix viewMat) = 0;
};

#endif
