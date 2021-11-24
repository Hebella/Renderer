#include "Model.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

Model::Model(const char *filename) 
{
	ifstream in;
	in.open(filename);
	if (in.fail())
		return;
	string line;
	char trash;
	while (!in.eof())
	{
		getline(in, line);
		istringstream iss(line.c_str());
		if (!line.compare(0, 2, "v "))
		{
			// 顶点坐标信息
			Vec3f v;
			iss >> trash >> v.raw[0] >> v.raw[1] >> v.raw[2];
			_vertex_coords.push_back(v);
		}
		else if (!line.compare(0, 2, "f ")) {
			// 面信息
			Vec3<int> vertex_idx, texture_idx, n_idx;
            int idx1, idx2, idx3, i = 0;
            iss >> trash;
            while (iss >> idx1 >> trash >> idx2 >> trash >> idx3) { // 第一项是顶点的序号 第二项是纹理顶点
                idx1--; // in wavefront obj all indices start at 1, not zero
				idx2--;
				idx3--;
				vertex_idx[i] = idx1;
				texture_idx[i] = idx2;
				n_idx[i] = idx3;
				i += 1;
            }
			_faces_vertex_idx.push_back(vertex_idx);
			_faces_texture_idx.push_back(texture_idx);
			_faces_n_idx.push_back(n_idx);
        }
		else if (!line.compare(0, 3, "vt "))
		{
			Vec2f t;
			iss >> trash >> trash >> t.raw[0] >> t.raw[1];
			_texture_coords.push_back(t);
		}
		else if (!line.compare(0, 3, "vn "))
		{
			Vec3f n;
			iss >> trash >> trash >> n.raw[0] >> n.raw[1] >> n.raw[2];
			_n_coords.push_back(n);
		}
    }
    std::cerr << "# v# " << _vertex_coords.size() << " f# "  << _faces_vertex_idx.size() << " vt# " << _texture_coords.size() << " vn# " << _n_coords.size() << std::endl;
}

Model::~Model()
{

}

int Model::nverts()
{
	return (int)_vertex_coords.size();
}

int Model::nfaces()
{
	return (int)_faces_vertex_idx.size();
}

Vec3<int> Model::getFaceVertexId(int idx)
{
	return _faces_vertex_idx[idx];
}

Vec3<int> Model::getFaceTextureId(int idx)
{
	return _faces_texture_idx[idx];
}

Vec3<int> Model::getFaceNormalId(int idx)
{
	return _faces_n_idx[idx];
}

Vec3f Model::getVertexCoords(int i)
{
	return _vertex_coords[i];
}

Vec2f Model::getTextureCoords(int i)
{
	return _texture_coords[i];
}

Vec3f Model::getNormalCoords(int i)
{
	return _n_coords[i];
}

void Model::initFragmentShader(int width, int height, const string filename, bool isSkyBox)
{
	fragShader = new FragmentShader(width, height, filename, isSkyBox);
}

void Model::initVertexShader(int width, int height, FragmentData*** zbuffer, unsigned char* framebuffer, Matrix projMat, Matrix modelMat, Matrix viewScreenMat, float proj_right, float proj_top, float proj_near, float proj_far)
{
	vertexShader = new VertexShader(width, height, zbuffer, fragShader, framebuffer, projMat, modelMat, viewScreenMat, proj_right, proj_top, proj_near, proj_far);
}

void Model::draw(Matrix viewMat)
{
	// render faces
	for (int i = 0; i < this->nfaces(); ++i)
	{
		Vec3<int> vertex_ids = this->getFaceVertexId(i); // 获取每个面上顶点的编号
		Vec3<int> texture_ids = this->getFaceTextureId(i); // 获取每个面上顶点对应的贴图的编号
		Vec3<int> normal_ids = this->getFaceNormalId(i); // 获取每个面上顶点对应的法向量的编号

		Vec3f vertex_coords[3]; // model_vertex_coords
		Vec2f texture_coords[3];
		Vec3f normal_coords[3]; // model_normal_coords

		for (int j = 0; j < 3; j++) {
			vertex_coords[j] = this->getVertexCoords(vertex_ids[j]);
			texture_coords[j] = this->getTextureCoords(texture_ids[j]);
			normal_coords[j] = this->getNormalCoords(normal_ids[j]);
		}

		vertexShader->Render(vertex_coords, texture_coords, normal_coords, viewMat);
	}
}