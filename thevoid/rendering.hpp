#pragma once
#include "nerdshit.hpp"

namespace rendering
{
	nerd_shit::fvec2_t setup_ortho();
	void restore_ortho();

	void initiate_render_hook();
	void uninitiate_render_hook();
}