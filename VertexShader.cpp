#include "VertexShader.h"
#include "FragmentData.h"



void VertexShader::triangle(Vec3f* vertex_coords, Vec2f* texture_coords, Vec3f* normal_coords, Vec3f* clipcoord_attri)
{
	Vec2f box_min(WIDTH - 1, HEIGHT - 1), box_max(0, 0), upper_bound(WIDTH - 1, HEIGHT - 1);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			box_min[j] = max(0.0f, min(box_min[j], vertex_coords[i][j]));
			box_max[j] = min(upper_bound[j], max(box_max[j], vertex_coords[i][j]));
		}
	}

	Vec3f P;
	for (P.x = box_min[0]; P.x <= box_max[0]; ++P.x)
	{
		for (P.y = box_min[1]; P.y <= box_max[1]; ++P.y)
		{
			Vec3f barycentric_coords = VertexShader::getBarycentricCoords(P, vertex_coords);
			if (barycentric_coords.x < -0.07 || barycentric_coords.y < -0.07 || barycentric_coords.z < -0.07)
				continue;
			P.z = barycentric_coords.z * vertex_coords[0].z + barycentric_coords.x * vertex_coords[1].z + barycentric_coords.y * vertex_coords[2].z;
			
			_fragShader->Render(P, barycentric_coords, texture_coords, normal_coords, _framebuffer, _zbuffer, clipcoord_attri);
		}
	}
}

Vec3f VertexShader::getBarycentricCoords(Vec3f P, Vec3f* vertex_coords)
{
	Vec3f u = Vec3f(vertex_coords[1].x - vertex_coords[0].x, vertex_coords[2].x - vertex_coords[0].x, vertex_coords[0].x - P.x) ^ Vec3f(vertex_coords[1].y - vertex_coords[0].y, vertex_coords[2].y - vertex_coords[0].y, vertex_coords[0].y - P.y); // (AB, AC, PA)
	if (abs(u.z) < 1) // 整数向量的叉乘也是整数向量
		return Vec3f(-1, 1, 1);
	return Vec3f(u.x / u.z, u.y / u.z, 1.0f - u.x / u.z - u.y / u.z); // 对应(u, v, 1 - u - v)
}

void VertexShader::Render(Vec3f* vertex_coords, Vec2f* texture_coords, Vec3f* normal_coords, Matrix viewMat)
{
	for (int i = 0; i < 3; ++i)
	{
		//cout << "1" << vertex_coords[i];
		vertex_coords[i] = VertexShader::MatrixToPoint(viewMat * _modelMat * VertexShader::PointToMatrix(vertex_coords[i]));
		normal_coords[i] = VertexShader::MatrixToVec(viewMat * _modelMat * VertexShader::VecToMatrix(normal_coords[i]));
		//cout << vertex_coords[i];
	}

	Vec3f P(0, 0, 0), N(_proj_near * _proj_near / _proj_right, 0, _proj_near);
	N = N.normalize();
	Plane clipping_plane(P, N);
	// 齐次剪裁
	vector<Vec3f> new_vertex_coords(vertex_coords, vertex_coords + 3);
	vector<Vec2f> new_texture_coords(texture_coords, texture_coords + 3);
	//new_vertex_coords = clip_with_plane(clipping_plane, new_vertex_coords, new_texture_coords);

	clipping_plane.N = Vec3f(-_proj_near * _proj_near / _proj_right, 0, _proj_near).normalize();
	//new_vertex_coords = clip_with_plane(clipping_plane, new_vertex_coords, new_texture_coords);

	clipping_plane.N = Vec3f(0, _proj_near * _proj_near / _proj_top, _proj_near).normalize();
	//new_vertex_coords = clip_with_plane(clipping_plane, new_vertex_coords, new_texture_coords);

	clipping_plane.N = Vec3f(0, -_proj_near * _proj_near / _proj_top, _proj_near).normalize();
	//new_vertex_coords = clip_with_plane(clipping_plane, new_vertex_coords, new_texture_coords);

	clipping_plane.N = Vec3f(0, 0, 1).normalize();
	//new_vertex_coords = clip_with_plane(clipping_plane, new_vertex_coords, new_texture_coords);

	if (new_vertex_coords.empty())
		return;

	for (int i = 0; i < new_vertex_coords.size() - 2; ++i) 
	{
		//构成三角面的3个顶点索引
		int index0 = 0;
		int index1 = i + 1;
		int index2 = i + 2;

		Vec3f clipcoord_attri[3];
		clipcoord_attri[0] = new_vertex_coords[index0];
		clipcoord_attri[1] = new_vertex_coords[index1];
		clipcoord_attri[2] = new_vertex_coords[index2];

		Vec2f clip_texture_coords[3];
		clip_texture_coords[0] = new_texture_coords[index0];
		clip_texture_coords[1] = new_texture_coords[index1];
		clip_texture_coords[2] = new_texture_coords[index2];

		Vec3f screen_coords[3];

		for (int i = 0; i < 3; ++i)
		{
			screen_coords[i] = VertexShader::MatrixToPoint( _viewScreenMat * _projMat * VertexShader::PointToMatrix(clipcoord_attri[i]));
		}

		this->triangle(screen_coords, clip_texture_coords, normal_coords, clipcoord_attri);
	}
}

vector<Vec3f> VertexShader::clip_with_plane(Plane clipping_plane, const vector<Vec3f> & vertex_coords, vector<Vec2f> & texture_coords)
{
	vector<Vec3f> new_vertex_coords;
	vector<Vec2f> new_texture_coords;

	for (int i = 0; i < vertex_coords.size(); ++i)
	{
		Vec3f pre_vertex = vertex_coords[(i - 1 + vertex_coords.size()) % vertex_coords.size()];
		Vec3f cur_vertex = vertex_coords[i];

		Vec2f pre_texture = texture_coords[(i - 1 + texture_coords.size()) % texture_coords.size()];
		Vec2f cur_texture = texture_coords[i];

		float d1 = (pre_vertex - clipping_plane.P) * clipping_plane.N;
		float d2 = (cur_vertex - clipping_plane.P) * clipping_plane.N;

		if (d1 * d2 < 0)
		{
			float t = abs(d1) / (abs(d1) + abs(d2));
			Vec3f new_vertex = pre_vertex + (cur_vertex - pre_vertex) * t;
			Vec2f new_texture = pre_texture + (cur_texture - pre_texture) * t;
			new_vertex_coords.push_back(new_vertex);
			new_texture_coords.push_back(new_texture);
		}

		if (d2 < 0)
		{
			new_vertex_coords.push_back(cur_vertex);
			new_texture_coords.push_back(cur_texture);
		}
	}

	texture_coords = new_texture_coords;
	return new_vertex_coords;
}
Vec3f VertexShader::MatrixToPoint(Matrix m)
{
	return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

Matrix VertexShader::PointToMatrix(Vec3f v)
{

	Matrix m(4, 1);
	m[0][0] = v.x;
	m[1][0] = v.y;
	m[2][0] = v.z;
	m[3][0] = 1.0f;
	return m;
}

Matrix VertexShader::VecToMatrix(Vec3f n)
{
	Matrix m(4, 1);
	m[0][0] = n.x;
	m[1][0] = n.y;
	m[2][0] = n.z;
	m[3][0] = 0.0f;
	return m;
}

Vec3f VertexShader::MatrixToVec(Matrix m)
{
	return Vec3f(m[0][0], m[1][0], m[2][0]);
}
