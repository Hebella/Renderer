#include "FragmentShader.h"
#include <iostream>
#include "tgaimage.h"
using namespace std;

/*static void set_color(unsigned char* framebuffer, int x, int y, TGAColor color)
{
	int i;
	int index = ((800 - y - 1) * 800 + x) * 4; // the origin for pixel is bottom-left, but the framebuffer index counts from top-left

	for (i = 0; i < 3; i++)
		framebuffer[index + i] = color.raw[i];
}*/

static Vec3f reflect(const Vec3f& v, const Vec3f& n)
{
	return v + n * 2 * (-(v * n));
}

static void cal_cubemap_uv(Vec3f direction, Vec2f& uv, int& face_index)
{
	float ma = 0, sc = 0, tc = 0;
	float abs_x = fabs(direction[0]), abs_y = fabs(direction[1]), abs_z = fabs(direction[2]);

	if (abs_x > abs_y && abs_x > abs_z)			/* major axis -> x */
	{
		ma = abs_x;
		if (direction.x > 0)					/* positive x */
		{
			face_index = 0;
			sc = +direction.z;
			tc = +direction.y;
		}
		else									/* negative x */
		{
			face_index = 1;
			sc = -direction.z;
			tc = +direction.y;
		}
	}
	else if (abs_y > abs_z)						/* major axis -> y */
	{
		ma = abs_y;
		if (direction.y > 0)					/* positive y */
		{
			face_index = 2;
			sc = +direction.x;
			tc = +direction.z;
		}
		else									/* negative y */
		{
			face_index = 3;
			sc = +direction.x;
			tc = -direction.z;
		}
	}
	else										/* major axis -> z */
	{
		ma = abs_z;
		if (direction.z > 0)					/* positive z */
		{
			face_index = 4;
			sc = -direction.x;
			tc = +direction.y;
		}
		else									/* negative z */
		{
			face_index = 5;
			sc = +direction.x;
			tc = +direction.y;
		}
	}

	uv[0] = (sc / ma + 1.0f) / 2.0f;
	uv[1] = (tc / ma + 1.0f) / 2.0f;

}

// load texture
FragmentShader::FragmentShader(int width, int height, const string filename, bool isSkyBox): _width(width), _height(height), _isSkyBox(isSkyBox)
{
	if (!_isSkyBox)
	{
		texture = new TGAImage(width, height, TGAImage::RGB);
		cerr << "Texture file " << filename << " loading " << (texture->read_tga_file(filename.c_str()) ? "ok" : "failed") << endl;
		texture->flip_vertically();
		_alpha = 255.0;
	}
	else // deal with skybox's textures
	{
		faces[0] = new TGAImage();
		cerr << "Texture file " << filename + "_left.tga" << " loading " << (faces[0]->read_tga_file((filename + "_left.tga").c_str()) ? "ok" : "failed") << endl;
		faces[0]->flip_vertically();

		faces[1] = new TGAImage();
		cerr << "Texture file " << filename + "_right.tga" << " loading " << (faces[1]->read_tga_file((filename + "_right.tga").c_str()) ? "ok" : "failed") << endl;
		faces[1]->flip_vertically();

		faces[2] = new TGAImage();
		cerr << "Texture file " << filename + "_top.tga" << " loading " << (faces[2]->read_tga_file((filename + "_top.tga").c_str()) ? "ok" : "failed") << endl;
		faces[2]->flip_vertically();

		faces[3] = new TGAImage();
		cerr << "Texture file " << filename + "_bottom.tga" << " loading " << (faces[3]->read_tga_file((filename + "_bottom.tga").c_str()) ? "ok" : "failed") << endl;
		faces[3]->flip_vertically();

		faces[4] = new TGAImage();
		cerr << "Texture file " << filename + "_front.tga" << " loading " << (faces[4]->read_tga_file((filename + "_front.tga").c_str()) ? "ok" : "failed") << endl;
		faces[4]->flip_vertically();

		faces[5] = new TGAImage();
		cerr << "Texture file " << filename + "_back.tga" << " loading " << (faces[5]->read_tga_file((filename + "_back.tga").c_str()) ? "ok" : "failed") << endl;
		faces[5]->flip_vertically();
	}
}

FragmentShader::FragmentShader(int width, int height, TGAColor c)
{
	_alpha = c.a;
	texture = new TGAImage(width, height, TGAImage::RGB);
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
			texture->set(x, y, c);
	}
	_light = Vec3f(1, 1, 1);
}

// P 正在渲染的点
// barycentric_coords P的重心坐标表示
// texture_coords P所在的三角形的三个顶点对应的texture坐标
void FragmentShader::Render(Vec3f P, Vec3f barycentric_coords, Vec2f* texture_coords, Vec3f* normal_coords, unsigned char* framebuffer, FragmentData*** zbuffer, Vec3f* clipcoord_attri)
{
	// 纹理
	Vec2f T;
	T.x = fmod(barycentric_coords.z * texture_coords[0].x + barycentric_coords.x * texture_coords[1].x + barycentric_coords.y * texture_coords[2].x, 1.0);
	T.y = fmod(barycentric_coords.z * texture_coords[0].y + barycentric_coords.x * texture_coords[1].y + barycentric_coords.y * texture_coords[2].y, 1.0);

	if (T.x >= 1.0 || T.y >= 1.0)
		cout << T.x << " " << T.y << endl;
	// 法向量
	Vec3f N;
	N.x = barycentric_coords.z * normal_coords[0].x + barycentric_coords.x * normal_coords[1].x + barycentric_coords.y * normal_coords[2].x;
	N.y = barycentric_coords.z * normal_coords[0].y + barycentric_coords.x * normal_coords[1].y + barycentric_coords.y * normal_coords[2].y;
	N.z = barycentric_coords.z * normal_coords[0].z + barycentric_coords.x * normal_coords[1].z + barycentric_coords.y * normal_coords[2].z;

	// 世界坐标系下的坐标 用来计算光照
	Vec3f world_P;
	world_P.x = barycentric_coords.z * clipcoord_attri[0].x + barycentric_coords.x * clipcoord_attri[1].x + barycentric_coords.y * clipcoord_attri[2].x;
	world_P.y = barycentric_coords.z * clipcoord_attri[0].y + barycentric_coords.x * clipcoord_attri[1].y + barycentric_coords.y * clipcoord_attri[2].y;
	world_P.z = barycentric_coords.z * clipcoord_attri[0].z + barycentric_coords.x * clipcoord_attri[1].z + barycentric_coords.y * clipcoord_attri[2].z;
	// 天空盒也是用这个坐标计算的

	if (_isSkyBox)
	{
		Vec2f uv;
		int face_index = -1;
		cal_cubemap_uv(world_P, uv, face_index);
		TGAColor c = faces[face_index]->get(uv.x * _width, uv.y * _height);
		FragmentData* element = new FragmentData(c, P.z, zbuffer[int(P.x)][int(P.y)]);
		zbuffer[int(P.x)][int(P.y)] = element;
		//cout << P.x << " " << P.y << endl;
		return;
	}

	Vec3f light_dir = (_light - world_P).normalize();
	float diff = max(0.0f, N.normalize() * light_dir);

	Vec3f view_dir(0, 0, 1);
	Vec3f halfway_dir = (light_dir + view_dir).normalize();
	float spec = pow(max(N.normalize() * halfway_dir, 0.0f), 32) * 0.2;

	TGAColor c = texture->get(T.x * _width, T.y * _height);
	TGAColor color;

	for (int i = 0; i < 3; ++i)
		color.raw[2 - i] = min(0.1 * c.raw[i] + c.raw[i] * diff + c.raw[i] * spec, 255.0);
		//color.raw[i] = c.raw[i];
	color.a = _alpha;
	//set_color(framebuffer, P.x, P.y, color);
	FragmentData* element = new FragmentData(color, P.z, zbuffer[int(P.x)][int(P.y)]);
	zbuffer[int(P.x)][int(P.y)] = element;

}





