#pragma once
#include <cstddef>
#include <cmath>

// All of the code here was written by fishy <3

// make false for row matrix multiplication (DirectX uses row, OpenGL uses column)
#define COLUMN_MODE true

// some games use the Z axis as up and down, mark this if your game is that (so that the rotation vectors get constructed right)
#define Z_IS_UP true

namespace nerd_shit
{
	struct fvec2_t
	{
		float x, y;

		fvec2_t() {};
		fvec2_t(float x, float y) : x{ static_cast<float>(x) }, y{ static_cast<float>(y) } {};

		float& operator[](std::size_t idx)
		{
			return *(&this->x + idx);
		}
	};

	struct fvec3_t : fvec2_t
	{
		float z;

		fvec3_t() {};
		fvec3_t(float x, float y, float z) : z{ static_cast<float>(z) }
		{
			this->x = x;
			this->y = y;
		}

		float magnitude()
		{
			return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
		}

		fvec3_t operator+(const fvec3_t& other)
		{
			fvec3_t output{};

			output.x = x + other.x;
			output.y = y + other.y;
			output.z = z + other.z;

			return output;
		}

		fvec3_t operator-(const fvec3_t& other)
		{
			fvec3_t output{};

			output.x = x - other.x;
			output.y = y - other.y;
			output.z = z - other.z;

			return output;
		}

		float& operator[](std::size_t idx)
		{
			return *(&this->x + idx);
		}
	};

	struct fvec4_t : fvec3_t
	{
		float w;

		fvec4_t() {};
		fvec4_t(float x, float y, float z, float w) : w{ static_cast<float>(w) }
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		
		fvec4_t(fvec2_t& one, fvec2_t& two)
		{
			this->x = one.x;
			this->y = one.y;

			this->z = two.x;
			this->w = two.y;
		}

		float& operator[](std::size_t idx)
		{
			return *(&this->x + idx);
		}
	};

	struct fmatrix_4x4_t
	{
		union
		{
			struct
			{
				#if Z_IS_UP
					fvec4_t right_vector;
					fvec4_t up_vector;
					fvec4_t back_vector;
					fvec4_t position;
				#else
					fvec4_t right_vector;
					fvec4_t back_vector;
					fvec4_t up_vector;
					fvec4_t position;
				#endif
			};

			float raw[16];
		};

		fmatrix_4x4_t()
		{
			// This needs tweaking depending on the game, just test with this code, and adjust according to what u see
			// Some games switch the back vector with forward vector, if this is the case make the back vector -1

			/*
				nerd_shit::fmatrix_4x4 center_point{};
				center_point.position = { 227, 37, 8, 1 }; // make this to a point u can be near

				nerd_shit::fmatrix_4x4 forward_point{};
				forward_point.position = { 0, 0, -5, 1 };

				nerd_shit::fmatrix_4x4 up_point{};
				up_point.position = { 0, 5, 0, 1 };

				nerd_shit::fmatrix_4x4 right_point{};
				right_point.position = { 5, 0, 0, 1 };

				nerd_shit::fvec2_t center{};
				nerd_shit::fvec2_t forward{};
				nerd_shit::fvec2_t up{};
				nerd_shit::fvec2_t right{};

				bool res1 = nerd_shit::world_to_screen(*view_matrix, center_point.position, screen_size, center);
				bool res2 = nerd_shit::world_to_screen(*view_matrix, (center_point * up_point).position, screen_size, up);
				bool res3 = nerd_shit::world_to_screen(*view_matrix, (center_point * forward_point).position, screen_size, forward);
				bool res4 = nerd_shit::world_to_screen(*view_matrix, (center_point * right_point).position, screen_size, right);
			*/


			// mod this depending on your game if not working right
			right_vector = { 1, 0, 0, 0 };
			up_vector = { 0, 1, 0, 0 };
			back_vector = { 0, 0, 1, 0 };
			position = { 0, 0, 0, 1 };
		}

		void set_position(fvec3_t pos)
		{
			this->position.x = pos.x;
			this->position.y = pos.y;
			this->position.z = pos.z;
			this->position.w = 1;
		}

		void print()
		{
			fmatrix_4x4_t& mat = *this;
			std::printf("%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n",
				mat[0], mat[4], mat[8], mat[12],
				mat[1], mat[5], mat[9], mat[13],
				mat[2], mat[6], mat[10], mat[14],
				mat[3], mat[7], mat[11], mat[15]
			);
		}

		float& operator[](std::size_t idx)
		{
			return this->raw[idx];
		}

		fvec4_t operator*(fvec4_t& vec4)
		{
			fmatrix_4x4_t& mat = *this;
			fvec4_t output{};

			#if COLUMN_MODE
				output[0] = (vec4[0] * mat[0]) + (vec4[1] * mat[4]) + (vec4[2] * mat[8]) + (vec4[3] * mat[12]);
				output[1] = (vec4[0] * mat[1]) + (vec4[1] * mat[5]) + (vec4[2] * mat[9]) + (vec4[3] * mat[13]);
				output[2] = (vec4[0] * mat[2]) + (vec4[1] * mat[6]) + (vec4[2] * mat[10]) + (vec4[3] * mat[14]);
				output[3] = (vec4[0] * mat[3]) + (vec4[1] * mat[7]) + (vec4[2] * mat[11]) + (vec4[3] * mat[15]);
			#else
				output[0] = (vec4[0] * mat[0]) + (vec4[1] * mat[1]) + (vec4[2] * mat[2]) + (vec4[3] * mat[3]);
				output[1] = (vec4[0] * mat[4]) + (vec4[1] * mat[5]) + (vec4[2] * mat[6]) + (vec4[3] * mat[7]);
				output[2] = (vec4[0] * mat[8]) + (vec4[1] * mat[9]) + (vec4[2] * mat[10]) + (vec4[3] * mat[11]);
				output[3] = (vec4[0] * mat[12]) + (vec4[1] * mat[13]) + (vec4[2] * mat[14]) + (vec4[3] * mat[15]);
			#endif

			return output;
		}

		fmatrix_4x4_t operator*(fmatrix_4x4_t& mat2)
		{
			fmatrix_4x4_t& mat1 = *this;
			fmatrix_4x4_t output{};

			#if COLUMN_MODE
				output[0] = (mat1[0] * mat2[0]) + (mat1[4] * mat2[1]) + (mat1[8] * mat2[2]) + (mat1[12] * mat2[3]);
				output[1] = (mat1[1] * mat2[0]) + (mat1[5] * mat2[1]) + (mat1[9] * mat2[2]) + (mat1[13] * mat2[3]);
				output[2] = (mat1[2] * mat2[0]) + (mat1[6] * mat2[1]) + (mat1[10] * mat2[2]) + (mat1[14] * mat2[3]);
				output[3] = (mat1[3] * mat2[0]) + (mat1[7] * mat2[1]) + (mat1[11] * mat2[2]) + (mat1[15] * mat2[3]);

				output[4] = (mat1[0] * mat2[4]) + (mat1[4] * mat2[5]) + (mat1[8] * mat2[6]) + (mat1[12] * mat2[7]);
				output[5] = (mat1[1] * mat2[4]) + (mat1[5] * mat2[5]) + (mat1[9] * mat2[6]) + (mat1[13] * mat2[7]);
				output[6] = (mat1[2] * mat2[4]) + (mat1[6] * mat2[5]) + (mat1[10] * mat2[6]) + (mat1[14] * mat2[7]);
				output[7] = (mat1[3] * mat2[4]) + (mat1[7] * mat2[5]) + (mat1[11] * mat2[6]) + (mat1[15] * mat2[7]);

				output[8] = (mat1[0] * mat2[8]) + (mat1[4] * mat2[9]) + (mat1[8] * mat2[10]) + (mat1[12] * mat2[11]);
				output[9] = (mat1[1] * mat2[8]) + (mat1[5] * mat2[9]) + (mat1[9] * mat2[10]) + (mat1[13] * mat2[11]);
				output[10] = (mat1[2] * mat2[8]) + (mat1[6] * mat2[9]) + (mat1[10] * mat2[10]) + (mat1[14] * mat2[11]);
				output[11] = (mat1[3] * mat2[8]) + (mat1[7] * mat2[9]) + (mat1[11] * mat2[10]) + (mat1[15] * mat2[11]);

				output[12] = (mat1[0] * mat2[12]) + (mat1[4] * mat2[13]) + (mat1[8] * mat2[14]) + (mat1[12] * mat2[15]);
				output[13] = (mat1[1] * mat2[12]) + (mat1[5] * mat2[13]) + (mat1[9] * mat2[14]) + (mat1[13] * mat2[15]);
				output[14] = (mat1[2] * mat2[12]) + (mat1[6] * mat2[13]) + (mat1[10] * mat2[14]) + (mat1[14] * mat2[15]);
				output[15] = (mat1[3] * mat2[12]) + (mat1[7] * mat2[13]) + (mat1[11] * mat2[14]) + (mat1[15] * mat2[15]);
			#else
				output[0] = (mat1[0] * mat2[0]) + (mat1[1] * mat2[4]) + (mat1[2] * mat2[8]) + (mat1[3] * mat2[12]);
				output[1] = (mat1[4] * mat2[0]) + (mat1[5] * mat2[4]) + (mat1[6] * mat2[8]) + (mat1[7] * mat2[12]);
				output[2] = (mat1[8] * mat2[0]) + (mat1[9] * mat2[4]) + (mat1[10] * mat2[8]) + (mat1[11] * mat2[12]);
				output[3] = (mat1[12] * mat2[0]) + (mat1[13] * mat2[4]) + (mat1[14] * mat2[8]) + (mat1[15] * mat2[12]);

				output[4] = (mat1[0] * mat2[1]) + (mat1[1] * mat2[5]) + (mat1[2] * mat2[9]) + (mat1[3] * mat2[13]);
				output[5] = (mat1[4] * mat2[1]) + (mat1[5] * mat2[5]) + (mat1[6] * mat2[9]) + (mat1[7] * mat2[13]);
				output[6] = (mat1[8] * mat2[1]) + (mat1[9] * mat2[5]) + (mat1[10] * mat2[9]) + (mat1[11] * mat2[13]);
				output[7] = (mat1[12] * mat2[1]) + (mat1[13] * mat2[5]) + (mat1[14] * mat2[9]) + (mat1[15] * mat2[13]);

				output[8] = (mat1[0] * mat2[2]) + (mat1[1] * mat2[6]) + (mat1[2] * mat2[10]) + (mat1[3] * mat2[13]);
				output[9] = (mat1[4] * mat2[2]) + (mat1[5] * mat2[6]) + (mat1[6] * mat2[10]) + (mat1[7] * mat2[13]);
				output[10] = (mat1[8] * mat2[2]) + (mat1[9] * mat2[6]) + (mat1[10] * mat2[10]) + (mat1[11] * mat2[13]);
				output[11] = (mat1[12] * mat2[2]) + (mat1[13] * mat2[6]) + (mat1[14] * mat2[10]) + (mat1[15] * mat2[13]);

				output[12] = (mat1[0] * mat2[3]) + (mat1[1] * mat2[7]) + (mat1[2] * mat2[11]) + (mat1[3] * mat2[14]);
				output[13] = (mat1[4] * mat2[3]) + (mat1[5] * mat2[7]) + (mat1[6] * mat2[11]) + (mat1[7] * mat2[14]);
				output[14] = (mat1[8] * mat2[3]) + (mat1[9] * mat2[7]) + (mat1[10] * mat2[11]) + (mat1[11] * mat2[14]);
				output[15] = (mat1[12] * mat2[3]) + (mat1[13] * mat2[7]) + (mat1[14] * mat2[11]) + (mat1[15] * mat2[14]);
			#endif

			return output;
		}

		/*
			* If you don't understand this matrix math this can help with the actual math side, with understand how matrices work in 3d space you just need to visualize it as something it isn't.
			* Understanding a matrix as an illusion is easier than getting into the numbers, I don't think of them as numbers but im capable of doing the math,
			* because of my mental mind of how they work in 3d space.
			* https://www.cuemath.com/algebra/multiplication-of-matrices/
			*
			* Math for this multiplication (this one is row multiplication):
				a b c d		a b c d
				e f g h		e f g h
				i j k l		i j k l
				m n o p		m n o p

				(aa + be + ci + dm)		(ab + bf + cj + dn)		(ac + bg + ck + do)		(ad + bh + cl + dp)
				(ea + fe + gi + hm)		(eb + ff + gj + hn)		(ec + fg + gk + ho)		(ed + fh + gl + hp)
				(ia + je + ki + lm)		(ib + jf + kj + ln)		(ic + jg + kk + lo)		(id + jh + kl + lp)
				(ma + ne + oi + pm)		(mb + nf + oj + pn)		(mc + ng + ok + po)		(md + nh + ol + pp)

			If you want to be successful in understanding this make sure you practice allot, I literally calculated the inverse of a 4x4 matrix
			by hand and it took me a long time to learn but that's the kind of practice which gets you places.
		*/
	};

	static bool world_to_screen(fmatrix_4x4_t& view_matrix, fvec3_t point, fvec2_t screen_size, fvec2_t& screen_coords)
	{
		fvec4_t fixed_point = { point.x, point.y, point.z, 1 }; // make it possible for matrix multiplication
		fvec4_t clip_coords = view_matrix * fixed_point;

		if (clip_coords.w < 0.1f)
			return false;

		fvec2_t normalized_device_coords{};
		normalized_device_coords.x = clip_coords.x / clip_coords.w;
		normalized_device_coords.y = clip_coords.y / clip_coords.w;

		// Will need modifications based on how your rendering engine stores coordinates in the 2nd dimension.
		screen_coords.x = (screen_size.x / 2 * normalized_device_coords.x) + normalized_device_coords.x + screen_size.x / 2;
		screen_coords.y = -(screen_size.y / 2 * normalized_device_coords.y) + normalized_device_coords.y + screen_size.y / 2;

		return true;
	}
}