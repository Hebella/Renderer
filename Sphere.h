#pragma once
#ifndef _SPHERE_H_
#define _SPHERE_H_
#include <vector>
#include "geometry.h"
#include "tgaimage.h"
class Sphere
{
private:
	vector<Vec3f> _vertex_coords; // ���ж��������
	vector<Vec2f> _texture_coords; // ��������
	vector<Vec3f> _n_coords; // ���㷨����������
	vector<Vec3<int>> _faces_vertex_idx; // ���Ӧ�Ķ�������ı��
	vector<Vec3<int>> _faces_texture_idx; //���Ӧ����������ı��
	vector<Vec3<int>> _faces_n_idx; //���Ӧ�ķ���������ı��
public:
	Sphere();
	~Sphere();
	int nverts(); // �������
	int nfaces(); // �����
	Vec3f getVertexCoords(int i); // ��ȡ���Ϊi�Ķ�������
	Vec2f getTextureCoords(int i); // ��ȡ���Ϊi�Ķ����Ӧ����������
	Vec3f getNormalCoords(int i);
	Vec3<int> getFaceVertexId(int idx); // ��ȡ���Ϊidx�����Ӧ�Ķ�����
	Vec3<int> getFaceTextureId(int idx); // ��ȡ���Ϊidx�����ϵĶ����Ӧ��������
	Vec3<int> getFaceNormalId(int idx);
};

#endif