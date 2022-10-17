#include <Windows.h>
#include <memory>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "hook.hpp"
#include "rendering.hpp"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

using SwapBuffers_t = BOOL(WINAPI*)(HDC);

nerd_shit::fmatrix_4x4_t* view_matrix = reinterpret_cast<nerd_shit::fmatrix_4x4_t*>(0x0057DFD0);


namespace render_magic
{
	// play on ac_arabian and it will generate a big arrow lmao
	// updated for assault cube version 1.3.0.2
	std::vector<nerd_shit::fvec4_t> generate_3d_box_coords(nerd_shit::fvec2_t screen_size, std::vector<nerd_shit::fvec3_t>& points, float size, std::vector<bool>& success)
	{
		std::vector<nerd_shit::fvec4_t> output{};

		float half_size = size / 2;

		for (nerd_shit::fvec3_t& center_point_v3 : points)
		{

			nerd_shit::fmatrix_4x4_t center_point{};
			center_point.set_position(center_point_v3);

			nerd_shit::fmatrix_4x4_t top_back_left_p{};
			top_back_left_p.set_position({ -half_size, -half_size, half_size });

			nerd_shit::fmatrix_4x4_t bottom_back_left_p{};
			bottom_back_left_p.set_position({ -half_size, -half_size, -half_size });

			nerd_shit::fmatrix_4x4_t top_back_right_p{};
			top_back_right_p.set_position({ half_size, -half_size, half_size });

			nerd_shit::fmatrix_4x4_t bottom_back_right_p{};
			bottom_back_right_p.set_position({ half_size, -half_size, -half_size });

			nerd_shit::fmatrix_4x4_t top_forward_left_p{};
			top_forward_left_p.set_position({ -half_size, half_size, half_size });

			nerd_shit::fmatrix_4x4_t bottom_forward_left_p{};
			bottom_forward_left_p.set_position({ -half_size, half_size, -half_size });

			nerd_shit::fmatrix_4x4_t top_forward_right_p{};
			top_forward_right_p.set_position({ half_size, half_size, half_size });

			nerd_shit::fmatrix_4x4_t bottom_forward_right_p{};
			bottom_forward_right_p.set_position({ half_size, half_size, -half_size });

			nerd_shit::fvec2_t center,
				top_back_left, top_back_right, top_forward_left, top_forward_right,
				bottom_back_left, bottom_back_right, bottom_forward_left, bottom_forward_right;

			if (
				nerd_shit::world_to_screen(*view_matrix, center_point.position, screen_size, center) &&
				nerd_shit::world_to_screen(*view_matrix, (center_point * top_back_left_p).position, screen_size, top_back_left) &&
				nerd_shit::world_to_screen(*view_matrix, (center_point * top_back_right_p).position, screen_size, top_back_right) &&
				nerd_shit::world_to_screen(*view_matrix, (center_point * top_forward_left_p).position, screen_size, top_forward_left) &&
				nerd_shit::world_to_screen(*view_matrix, (center_point * top_forward_right_p).position, screen_size, top_forward_right) &&

				nerd_shit::world_to_screen(*view_matrix, (center_point * bottom_back_left_p).position, screen_size, bottom_back_left) &&
				nerd_shit::world_to_screen(*view_matrix, (center_point * bottom_back_right_p).position, screen_size, bottom_back_right) &&
				nerd_shit::world_to_screen(*view_matrix, (center_point * bottom_forward_left_p).position, screen_size, bottom_forward_left) &&
				nerd_shit::world_to_screen(*view_matrix, (center_point * bottom_forward_right_p).position, screen_size, bottom_forward_right)
				)
			{
				success.push_back(true);

				nerd_shit::fvec2_t screen_center;
				screen_center.x = screen_size.x / 2;
				screen_center.y = screen_size.y;

				// output.emplace_back(screen_center, center);

				// draw top square
				output.emplace_back(top_back_left, top_back_right);
				output.emplace_back(top_forward_left, top_forward_right);
				output.emplace_back(top_back_left, top_forward_left);
				output.emplace_back(top_back_right, top_forward_right);

				// draw bottom square
				output.emplace_back(bottom_back_left, bottom_back_right);
				output.emplace_back(bottom_forward_left, bottom_forward_right);
				output.emplace_back(bottom_back_left, bottom_forward_left);
				output.emplace_back(bottom_back_right, bottom_forward_right);

				// draw lines to link squares
				output.emplace_back(bottom_back_left, top_back_left);
				output.emplace_back(bottom_back_right, top_back_right);
				output.emplace_back(bottom_forward_left, top_forward_left);
				output.emplace_back(bottom_forward_right, top_forward_right);
			}
			else
			{
				success.push_back(false);
			}
		}

		return output;
	}

	void render_big_arrow()
	{
		nerd_shit::fvec2_t screen_size = rendering::setup_ortho();
		{
			std::vector<nerd_shit::fvec3_t> center_points{};

			center_points.emplace_back(227, 31 + 2, 2);

			center_points.emplace_back(227, 31 + 4, 2);
			center_points.emplace_back(227, 31 + 6, 2);
			center_points.emplace_back(227, 31 + 6, 4);

			center_points.emplace_back(227, 31 + 8, 2);
			center_points.emplace_back(227, 31 + 10, 2);
			center_points.emplace_back(227, 31 + 10, 4);

			center_points.emplace_back(227, 31 + 12, 2);
			center_points.emplace_back(227, 31 + 14, 2);
			center_points.emplace_back(227, 31 + 14, 4);

			//

			center_points.emplace_back(227 - 2, 31 + 2, 2);
			center_points.emplace_back(227 - 2, 31 + 2, 4);

			center_points.emplace_back(227 - 4, 31 + 4, 2);
			center_points.emplace_back(227 - 6, 31 + 6, 2);
			center_points.emplace_back(227 - 6, 31 + 6, 4);

			center_points.emplace_back(227 - 8, 31 + 8, 2);
			center_points.emplace_back(227 - 10, 31 + 10, 2);
			center_points.emplace_back(227 - 10, 31 + 10, 4);

			center_points.emplace_back(227 - 12, 31 + 12, 2);
			center_points.emplace_back(227 - 14, 31 + 14, 2);
			center_points.emplace_back(227 - 14, 31 + 14, 4);

			//

			center_points.emplace_back(227 - 2, 31, 2);
			center_points.emplace_back(227 - 2, 31, 4);

			center_points.emplace_back(227 - 4, 31, 2);
			center_points.emplace_back(227 - 6, 31, 2);
			center_points.emplace_back(227 - 6, 31, 4);

			center_points.emplace_back(227 - 8, 31, 2);
			center_points.emplace_back(227 - 10, 31, 2);
			center_points.emplace_back(227 - 10, 31 + 10, 4);

			center_points.emplace_back(227 - 12, 31, 2);
			center_points.emplace_back(227 - 14, 31, 2);
			center_points.emplace_back(227 - 14, 31, 4);

			std::vector<bool> success;
			std::vector<nerd_shit::fvec4_t> render_points = generate_3d_box_coords(screen_size, center_points, 2, success);

			glPushAttrib(GL_LINE_WIDTH);
			{
				glColor4ub(255, 0, 255, 255);
				glLineWidth(3);

				glBegin(GL_LINES);
				{
					std::uint32_t i = 0;
					for (bool status : success)
					{
						if (status)
						{
							std::uint32_t goal = i + 12;
							for (i; i < goal; ++i)
							{
								nerd_shit::fvec4_t& curr = render_points[i];
								glVertex2f(curr.x, curr.y);
								glVertex2f(curr.z, curr.w);
							}
						}
					}
				}
				glEnd();
			}
			glPopAttrib();
		}
		rendering::restore_ortho();
	}
}

namespace rendering
{
	nerd_shit::fvec2_t setup_ortho()
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();

		GLfloat viewport[4];
		glGetFloatv(GL_VIEWPORT, viewport);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluOrtho2D(0, viewport[2], viewport[3], 0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

		return { viewport[2], viewport[3] };
	}

	void restore_ortho()
	{
		glPopAttrib();
		glPopMatrix();
	}

	std::unique_ptr<hook_t<SwapBuffers_t>> my_hook = std::make_unique<hook_t<SwapBuffers_t>>(&SwapBuffers, 6);

	SwapBuffers_t original_SwapBuffers;
	BOOL WINAPI SwapBuffers_hook(HDC unnamedParam1)
	{
		if (view_matrix != nullptr)
		{
			render_magic::render_big_arrow();
		}
		return original_SwapBuffers(unnamedParam1);
	}

	void initiate_render_hook()
	{
		my_hook->enable_hook(&SwapBuffers_hook, original_SwapBuffers);
	}

	void uninitiate_render_hook()
	{
		my_hook->disable_hook();
		my_hook.release();
	}
}