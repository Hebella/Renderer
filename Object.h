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
	vector<Vec3f> _vertex_coords; // ���ж��������
	vector<Vec2f> _texture_coords; // ��������
	vector<Vec3f> _n_coords; // ���㷨����������
	vector<Vec3<int>> _faces_vertex_idx; // ���Ӧ�Ķ�������ı��
	vector<Vec3<int>> _faces_texture_idx; //���Ӧ����������ı��
	vector<Vec3<int>> _faces_n_idx; //���Ӧ�ķ���������ı��
public:
	virtual ~Object();
	virtual int nverts() = 0; // �������
	virtual int nfaces() = 0; // �����
	virtual Vec3f getVertexCoords(int i) = 0; // ��ȡ���Ϊi�Ķ�������
	virtual Vec2f getTextureCoords(int i) = 0; // ��ȡ���Ϊi�Ķ����Ӧ����������
	virtual Vec3f getNormalCoords(int i) = 0;
	virtual Vec3<int> getFaceVertexId(int idx) = 0; // ��ȡ���Ϊidx�����Ӧ�Ķ�����
	virtual Vec3<int> getFaceTextureId(int idx) = 0; // ��ȡ���Ϊidx�����ϵĶ����Ӧ��������
	virtual Vec3<int> getFaceNormalId(int idx) = 0;
	virtual void draw(Matrix viewMat) = 0;
};

#endif
