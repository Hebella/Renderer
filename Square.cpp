#include "Square.h"

Square::Square() {
	// 八个顶点
	_vertex_coords.push_back(Vec3f(-0.5, 0.5, 0.5));
	_vertex_coords.push_back(Vec3f(0.5, 0.5, 0.5));
	_vertex_coords.push_back(Vec3f(0.5, -0.5, 0.5));
	_vertex_coords.push_back(Vec3f(-0.5, -0.5, 0.5));
	_vertex_coords.push_back(Vec3f(-0.5, 0.5, -0.5));
	_vertex_coords.push_back(Vec3f(0.5, 0.5, -0.5));
	_vertex_coords.push_back(Vec3f(0.5, -0.5, -0.5));
	_vertex_coords.push_back(Vec3f(-0.5, -0.5, -0.5));

	_texture_coords.push_back(Vec2f(0.0, 0.0));

	_n_coords.push_back(Vec3f( 0, 0, 1)); // front
	_n_coords.push_back(Vec3f( 0, 0, 1));
	_n_coords.push_back(Vec3f( 1, 0, 0)); // right
	_n_coords.push_back(Vec3f( 1, 0, 0));
	_n_coords.push_back(Vec3f( 0, 1, 0)); // top
	_n_coords.push_back(Vec3f( 0, 1, 0));
	_n_coords.push_back(Vec3f( 0, 0,-1)); // back
	_n_coords.push_back(Vec3f( 0, 0,-1));
	_n_coords.push_back(Vec3f(-1, 0, 0)); // left
	_n_coords.push_back(Vec3f(-1, 0, 0));
	_n_coords.push_back(Vec3f( 0,-1, 0)); // bottom
	_n_coords.push_back(Vec3f( 0,-1, 0));

	_faces_vertex_idx.push_back(Vec3<int>(0, 1, 3)); // front 0 1
	_faces_vertex_idx.push_back(Vec3<int>(1, 2, 3)); //       3 2
	_faces_vertex_idx.push_back(Vec3<int>(1, 5, 2)); // right 1 5
	_faces_vertex_idx.push_back(Vec3<int>(5, 6, 2)); //       2 6
	_faces_vertex_idx.push_back(Vec3<int>(4, 5, 0)); // top   4 5
	_faces_vertex_idx.push_back(Vec3<int>(5, 1, 0)); //       0 1
	_faces_vertex_idx.push_back(Vec3<int>(5, 4, 6)); // back  5 4
	_faces_vertex_idx.push_back(Vec3<int>(4, 7, 6)); //       6 7
	_faces_vertex_idx.push_back(Vec3<int>(4, 0, 7)); // left  4 0
	_faces_vertex_idx.push_back(Vec3<int>(0, 3, 7)); //       7 3
	_faces_vertex_idx.push_back(Vec3<int>(3, 2, 7)); // bottom 3 2
	_faces_vertex_idx.push_back(Vec3<int>(2, 6, 7)); //        7 6

	_faces_texture_idx.push_back(Vec3<int>(0, 0, 0)); 

	_faces_n_idx.push_back(Vec3<int>(0, 0, 0)); // front 0 1
	_faces_n_idx.push_back(Vec3<int>(1, 1, 1)); //       3 2
	_faces_n_idx.push_back(Vec3<int>(2, 2, 2)); // right 1 5
	_faces_n_idx.push_back(Vec3<int>(3, 3, 3)); //       2 6
	_faces_n_idx.push_back(Vec3<int>(4, 4, 4)); // top   4 5
	_faces_n_idx.push_back(Vec3<int>(5, 5, 5)); //       0 1
	_faces_n_idx.push_back(Vec3<int>(6, 6, 6)); // back  5 4
	_faces_n_idx.push_back(Vec3<int>(7, 7, 7)); //       6 7
	_faces_n_idx.push_back(Vec3<int>(8, 8, 8)); // left  4 0
	_faces_n_idx.push_back(Vec3<int>(9, 9, 9)); //       7 3
	_faces_n_idx.push_back(Vec3<int>(10, 10, 10)); // bottom 3 2
	_faces_n_idx.push_back(Vec3<int>(11, 11, 11)); //        7 6

}

Square::~Square()
{

}

int Square::nverts()
{
	return (int)_vertex_coords.size();
}

int Square::nfaces()
{
	return (int)_faces_vertex_idx.size();
}

Vec3<int> Square::getFaceVertexId(int idx)
{
	return _faces_vertex_idx[idx];
}

Vec3<int> Square::getFaceTextureId(int idx)
{
	return _faces_texture_idx[0];
}

Vec3<int> Square::getFaceNormalId(int idx)
{
	return _faces_n_idx[idx];
}


Vec3f Square::getVertexCoords(int i)
{
	return _vertex_coords[i];
}

Vec2f Square::getTextureCoords(int i)
{
	return _texture_coords[0];
}

Vec3f Square::getNormalCoords(int i)
{
	return _n_coords[i];
}

void Square::initFragmentShader(TGAColor c)
{
	fragShader = new FragmentShader(5, 5, c);
}

void Square::initVertexShader(int width, int height, FragmentData*** zbuffer, unsigned char* framebuffer, Matrix projMat, Matrix modelMat, Matrix viewScreenMat, float proj_right, float proj_top, float proj_near, float proj_far)
{
	vertexShader = new VertexShader(width, height, zbuffer, fragShader, framebuffer, projMat, modelMat, viewScreenMat, proj_right, proj_top, proj_near, proj_far);
}

void Square::draw(Matrix viewMat)
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