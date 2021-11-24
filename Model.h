#pragma once
#ifndef  _MODEL_H_
#define _MODEL_H_

#include <vector>
#include "geometry.h"
#include "tgaimage.h"
#include "Object.h"

using namespace std;

class Model: public Object
{
private:
	VertexShader* vertexShader;
	FragmentShader* fragShader;
public:
	Model(const char* filename);
	~Model();
	virtual int nverts(); // 顶点个数
	virtual int nfaces(); // 面个数
	virtual Vec3f getVertexCoords(int i); // 获取编号为i的顶点坐标
	virtual Vec2f getTextureCoords(int i); // 获取编号为i的顶点对应的纹理坐标
	virtual Vec3f getNormalCoords(int i);
	virtual Vec3<int> getFaceVertexId(int idx); // 获取编号为idx的面对应的顶点编号
	virtual Vec3<int> getFaceTextureId(int idx); // 获取编号为idx的面上的顶点对应的纹理编号
	virtual Vec3<int> getFaceNormalId(int idx);

	void initVertexShader(int width, int height, FragmentData*** zbuffer, unsigned char* framebuffer, Matrix projMat, Matrix modelMat, Matrix viewScreenMat, float proj_right, float proj_top, float proj_near, float proj_far);
	void initFragmentShader(int width, int height, const string filename, bool isSkyBox = false);

	virtual void draw(Matrix viewMat);
};

#endif // ! _MODEL_H_



