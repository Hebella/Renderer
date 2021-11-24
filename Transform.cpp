#include "Transform.h"

// 将点的齐次坐标转化为非齐次坐标
Vec3f Transform::MatrixToVec(Matrix m)
{
	return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

// 将点的非齐次坐标转化为齐次坐标
Matrix Transform::VecToMatrix(Vec3f v)
{
	Matrix m(4, 1);
	m[0][0] = v.x;
	m[1][0] = v.y;
	m[2][0] = v.z;
	m[3][0] = 1.0f;
	return m;
}

// 将[-1, 1, -1, 1, -1, 1]映射到[left, left + w, top, top + h, 0, depth]
Matrix Transform::ViewToScreen(int left, int top, int w, int h, int depth) {
	Matrix m = Matrix::identity(4);
	m[0][3] = left + w / 2.f;
	m[1][3] = top + h / 2.f;
	m[2][3] = depth / 2.f;

	m[0][0] = w / 2.f;
	m[1][1] = h / 2.f;
	m[2][2] = depth / 2.f;
	return m;
}

// eye：摄像机所在的地方
// center：变换后的标架原点所在的地方
// http://www.songho.ca/opengl/gl_camera.html
Matrix Transform::LookAt(Vec3f eye, Vec3f target, Vec3f up)
{
	Vec3f z_ = (eye - target).normalize(); // the forward vector is from the target to the eye position
	Vec3f x_ = (up ^ z_).normalize();
	Vec3f y_ = (z_ ^ x_).normalize();
	Matrix mat1 = Matrix::identity(4);
	Matrix mat2 = Matrix::identity(4);
	for (int i = 0; i < 3; ++i)
	{
		mat1[0][i] = x_[i];
		mat1[1][i] = y_[i];
		mat1[2][i] = z_[i];
		mat2[i][3] = -eye[i];
	}
	return mat1 * mat2;
}

Matrix Transform::Projection(Vec3f camera)
{
	Matrix m = Matrix::identity(4);
	m[3][2] = -1.0f / camera.z;
	return m;
}

Matrix Transform::Projection(float r, float t, float n, float f)
{
	Matrix m = Matrix::identity(4);
	m[0][0] = n / r;
	m[1][1] = n / t;
	m[2][2] = (f + n) / (f - n);
	m[2][3] = 2 * f * n / (f - n);
	m[3][2] = -1;
	m[3][3] = 0;
	return m;
}

Matrix Transform::Scale(float sx, float sy, float sz)
{
	Matrix m = Matrix::identity(4);
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
	return m;
}

Matrix Transform::Translate(float tx, float ty, float tz)
{
	Matrix m = Matrix::identity(4);
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;
	return m;
}