#include "MyTriangle.h"

using namespace std;

void MyTriangle::triangle_1(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color)
{
	if (t0.y > t1.y)
		swap(t0, t1);
	if (t0.y > t2.y)
		swap(t0, t2);
	if (t1.y > t2.y)
		swap(t1, t2);
	// 按照t2.y > t1.y > t0.y排序
	int total_height = t2.y - t0.y;
	for (int y = t0.y; y <= t1.y; ++y) // 先画下面一半
	{
		float alpha = (y - t0.y) / (float)(t1.y - t0.y); // 较短的那条边
		float beta = (y - t0.y) / (float)(t2.y - t0.y); // 较长的那条边
		int x1 = t0.x + alpha * (t1.x - t0.x);
		int x2 = t0.x + beta * (t2.x - t0.x);
		if (x1 > x2)
			swap(x1, x2);
		for (int x = x1; x <= x2; ++x)
			image.set(x, y, color);
	}
	for (int y = t1.y; y <= t2.y; ++y) // 再画上面一半
	{
		float alpha = (y - t1.y) / (float)(t2.y - t1.y); // 较短的那条边
		float beta = (y - t0.y) / (float)(t2.y - t0.y); // 较长的那条边
		int x1 = t1.x + alpha * (t2.x - t1.x);
		int x2 = t0.x + beta * (t2.x - t0.x);
		if (x1 > x2)
			swap(x1, x2);
		for (int x = x1; x <= x2; ++x)
			image.set(x, y, color);
	}
}

void MyTriangle::triangle_2(Vec3f *pts, TGAImage& image, TGAImage& texture, int width, int height, float **zbuffer, Vec2f *face_tex)
{
	Vec2f box_min(width - 1, height - 1), box_max(0, 0), upper_bound(width - 1, height - 1);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			box_min.raw[j] = max(0.0f, min(box_min.raw[j], pts[i].raw[j]));
			box_max.raw[j] = min(upper_bound.raw[j], max(box_max.raw[j], pts[i].raw[j]));
		}
	}

	float subsample[4][2] =
	{
		{-0.25,-0.25},
		{ 0.25,-0.25},
		{-0.25, 0.25},
		{ 0.25, 0.25},
	};

	Vec3f P;
	for (P.x = box_min.raw[0] + 0.5; P.x <= box_max.raw[0] + 0.5; ++P.x)
	{
		for (P.y = box_min.raw[1] + 0.5; P.y <= box_max.raw[1] + 0.5; ++P.y)
		{
			int count = 0;

			for (int i = 0; i < 4; i++)
			{
				Vec3f sub_P;
				sub_P.x = P.x + subsample[i][0];
				sub_P.y = P.y + subsample[i][1];

				Vec3f sub_barycentric_coords = getBarycentricCoords(sub_P, pts);
				if (sub_barycentric_coords.x < 0 || sub_barycentric_coords.y < 0 || sub_barycentric_coords.z < 0)
					continue;

				count += 1;
			}

			Vec3f barycentric_coords = getBarycentricCoords(P, pts);
			if (barycentric_coords.x < 0 || barycentric_coords.y < 0 || barycentric_coords.z < 0)
				continue;
			P.z = barycentric_coords.z * pts[0].z + barycentric_coords.x * pts[1].z + barycentric_coords.y * pts[2].z;
			if (zbuffer[int(P.x)][int(P.y)] < P.z) // P在靠近摄像机的地方
			{
				zbuffer[int(P.x)][int(P.y)] = P.z;
				// 从材质中获取颜色
				Vec2f T;
				T.x = barycentric_coords.z * face_tex[0].x + barycentric_coords.x * face_tex[1].x + barycentric_coords.y * face_tex[2].x;
				T.y = barycentric_coords.z * face_tex[0].y + barycentric_coords.x * face_tex[1].y + barycentric_coords.y * face_tex[2].y;
				Vec2i T_(T.x * 1024, T.y * 1024);
				TGAColor c = texture.get(T_.x, T_.y);
				c.a = count / 4.0;
				cout << c.a << endl;
				image.set(P.x, P.y, c);
			}
		}
	}

}

Vec3f MyTriangle::getBarycentricCoords(Vec3f P, Vec3f* pts)
{
	Vec3f u = Vec3f(pts[1].x - pts[0].x, pts[2].x - pts[0].x, pts[0].x - P.x) ^ Vec3f(pts[1].y - pts[0].y, pts[2].y - pts[0].y, pts[0].y - P.y); // (AB, AC, PA)
	if (abs(u.z) < 1) // 整数向量的叉乘也是整数向量
		return Vec3f(-1, 1, 1);
	return Vec3f(u.x / u.z, u.y / u.z, 1.0f - u.x / u.z - u.y / u.z); // 对应(u, v, 1 - u - v)
}
