#pragma once
#ifndef _SQUARE_H_
#define _SQUARE_H_
#include "object.h"
#include "FragmentData.h"

class Square: public Object
{
private:
	VertexShader* vertexShader;
	FragmentShader* fragShader;
public:
	Square();
	~Square();
	virtual int nverts(); // 顶点个数
	virtual int nfaces(); // 面个数
	virtual Vec3f getVertexCoords(int i); // 获取编号为i的顶点坐标
	virtual Vec2f getTextureCoords(int i); // 获取编号为i的顶点对应的纹理坐标
	virtual Vec3f getNormalCoords(int i);
	virtual Vec3<int> getFaceVertexId(int idx); // 获取编号为idx的面对应的顶点编号
	virtual Vec3<int> getFaceTextureId(int idx); // 获取编号为idx的面上的顶点对应的纹理编号
	virtual Vec3<int> getFaceNormalId(int idx);

	void initVertexShader(int width, int height, FragmentData*** zbuffer, unsigned char* framebuffer, Matrix projMat, Matrix modelMat, Matrix viewScreenMat, float proj_right, float proj_top, float proj_near, float proj_far);
	void initFragmentShader(TGAColor c);

	virtual void draw(Matrix viewMat);
};

#endif

