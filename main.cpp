#include <iostream>
#include "Square.h"
#include "Model.h"
#include "win32.h"
#include "Transform.h"
#include "FragmentData.h"
#include <vector>
#include <algorithm>

#define EPSILON 1e-5f

using namespace std;

const int width = 800;
const int height = 800;
const int depth = 255;
const float proj_right = 0.15;
const float proj_top = 0.15;
const float proj_near = 0.25;
const float proj_far = 10000;

void clear_zbuffer(int width, int height, FragmentData*** zbuffer);
void clear_framebuffer(int width, int height, unsigned char* framebuffer);
void handle_mouse_events(Vec3f& eye, Vec3f& center);
void update_eye(Vec3f& eye, Vec3f& center);
void set_color(unsigned char* framebuffer, int x, int y, TGAColor color)
{
	int i;
	int index = ((800 - y - 1) * 800 + x) * 4; // the origin for pixel is bottom-left, but the framebuffer index counts from top-left

	for (i = 0; i < 4; i++)
		framebuffer[index + i] = color.raw[i];
}

struct fragmentColor
{
	float depth;
	TGAColor color;
	fragmentColor(float d = -10000.0f, TGAColor c = TGAColor(255, 255, 255, 255))
	{
		depth = d;
		color = c;
	}
};


static bool myCompare(const fragmentColor& f1, const fragmentColor& f2)
{
	return f1.depth < f2.depth;
}

int main()
{
	TGAImage image(width, height, TGAImage::RGB);
	Square model_1, model_2;
	Model model_3("helmet.obj"), model_4("box.obj");

	unsigned char* framebuffer = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 4);
	memset(framebuffer, 0, sizeof(unsigned char) * width * height * 4);

	FragmentData*** zbuffer = new FragmentData** [width]; // 每个元素都是一个指向FragmentData的指针
	for (int i = 0; i < width; ++i)
	{
		zbuffer[i] = new FragmentData* [height];
		for (int j = 0; j < height; ++j)
		{
			zbuffer[i][j] = NULL;
		}
	}

	TGAColor c1(218, 201, 248, 196);
	model_1.initFragmentShader(c1);

	TGAColor c2(230, 224, 176, 128);
	model_2.initFragmentShader(c2);

	model_3.initFragmentShader(1024, 1024, "helmet_diffuse.tga");
	
	model_4.initFragmentShader(256, 256, "box", true);

	Vec3f camera(0, 0, 1.0f);
	Vec3f eye(0.0, 0.0, 1.0);
	Vec3f target(0, 0, 0);

	Matrix projMat = Transform::Projection(proj_right, proj_top, proj_near, proj_far);
	//Matrix projMat = Transform::Projection(camera);

	Matrix viewScreenMat = Transform::ViewToScreen(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);

	Matrix translateMat_1 = Transform::Translate(0, 0.0, -1);
	Matrix scaleMat_1 = Transform::Scale(0.5, 0.5, 0.5);
	Matrix modelMat_1 = translateMat_1 * scaleMat_1;

	Matrix translateMat_2 = Transform::Translate(0.25, 0, 0);
	Matrix scaleMat_2 = Transform::Scale(0.1, 1, 0.5);
	Matrix modelMat_2 = translateMat_2 * scaleMat_2;

	Matrix translateMat_3 = Transform::Translate(0, 0, 0);
	Matrix scaleMat_3 = Transform::Scale(0.5, 0.5, 0.5);
	Matrix modelMat_3 = translateMat_3 * scaleMat_3;

	Matrix modelMat_4 = Transform::Translate(0, 0, 1);

	model_1.initVertexShader(width, height, zbuffer, framebuffer, projMat, modelMat_1, viewScreenMat, proj_right, proj_top, proj_near, proj_far);
	model_2.initVertexShader(width, height, zbuffer, framebuffer, projMat, modelMat_2, viewScreenMat, proj_right, proj_top, proj_near, proj_far);
	model_3.initVertexShader(width, height, zbuffer, framebuffer, projMat, modelMat_3, viewScreenMat, proj_right, proj_top, proj_near, proj_far);
	model_4.initVertexShader(width, height, zbuffer, framebuffer, projMat, modelMat_3, viewScreenMat, proj_right, proj_top, proj_near, proj_far);

	window_init(width, height, "SRender");

	int num_frames = 0;
	float print_time = platform_get_time();
	while (!window->is_close)
	{
		float curr_time = platform_get_time();
		clear_zbuffer(width, height, zbuffer);
		clear_framebuffer(width, height, framebuffer);

		handle_mouse_events(eye, target);
		Matrix viewMat = Transform::LookAt(eye, target, Vec3f(0, 1, 0));

		//model_1.draw(viewMat);
		// model_2.draw(viewMat);
		//model_3.draw(viewMat);
		model_4.draw(viewMat);

		int cnt = 0;
		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				if (zbuffer[x][y] != NULL)
				{
					// 深度由小到大排列
					//vector<fragmentColor> color_vector;
					FragmentData* p = zbuffer[x][y];

					TGAColor cur_color(0, 0, 0, 255);
					float max_depth = -20000;
					while (p != NULL)
					{
						if (p->_depth > max_depth)
						{
							max_depth = p->_depth;
							cur_color = p->_color;
						}
						//fragmentColor element(p->_depth, p->_color);
						//color_vector.push_back(element);
						p = p->_next;
					}
					//sort(color_vector.begin(), color_vector.end(), myCompare);
					//TGAColor cur_color(0, 0, 0, 255);
					//int cur_depth = -100000;
					//for (int i = 0; i < color_vector.size(); ++i)
					//{
					//	if (cur_depth != int(color_vector[i].depth)) // 消除重复渲染的位于面的边界上的点
					//	{
					//		cur_color.alpha_blending(color_vector[i].color);
					//		cur_depth = int(color_vector[i].depth);
					//	}
						
					//}
					set_color(framebuffer, x, y, cur_color);
				}
			}
		}


		num_frames += 1;
		if (curr_time - print_time >= 1) {
			int sum_millis = (int)((curr_time - print_time) * 1000);
			int avg_millis = sum_millis / num_frames;
			printf("fps: %3d, avg: %3d ms\n", num_frames, avg_millis);
			num_frames = 0;
			print_time = curr_time;
		}
		
		window->mouse_info.orbit_delta = Vec2f(0.0, 0.0);
		window_draw(framebuffer);
		msg_dispatch();


	}
	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}

void clear_zbuffer(int width, int height, FragmentData*** zbuffer)
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			if (zbuffer[i][j] != NULL)
			{
				FragmentData* head = zbuffer[i][j];
				FragmentData* p;
				while (head->_next != NULL)
				{
					p = head->_next;
					delete head;
					head = p;
				}
				delete head;
			}
			zbuffer[i][j] = NULL;
		}
	}
}

void clear_framebuffer(int width, int height, unsigned char* framebuffer)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int index = (i * width + j) * 4;

			framebuffer[index + 2] = 0;
			framebuffer[index + 1] = 0;
			framebuffer[index] = 0;
		}
	}
}

void handle_mouse_events(Vec3f & eye, Vec3f & center)
{
	if (window->buttons[0])
	{
		Vec2f cur_pos = get_mouse_pos();
		window->mouse_info.orbit_delta = window->mouse_info.orbit_pos - cur_pos;
		window->mouse_info.orbit_pos = cur_pos;
	}

	update_eye(eye, center);
}

void update_eye(Vec3f& eye, Vec3f& center)
{
	Vec3f from_center = eye - center;
	float distance = from_center.norm();

	float phi = (float)atan2(from_center[0], from_center[2]); // 在x-z平面内的投影和z轴的夹角
	float theta = (float)acos(from_center[1] / distance);
	float xz_delta = window->mouse_info.orbit_delta[0] / width;
	float yz_delta = window->mouse_info.orbit_delta[1] / height;

	float factor = 1.5 * 3.14;

	phi += xz_delta * factor;
	theta += yz_delta * factor;
	if (theta > 3.14)
		theta = 3.14 - EPSILON * 100;
	if (theta < 0)
		theta = EPSILON * 100;

	eye[0] = center[0] + distance * sin(theta) * sin(phi);
	eye[1] = center[1] + distance * cos(theta);
	eye[2] = center[2] + distance * sin(theta) * cos(phi);
}