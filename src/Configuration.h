#pragma once
#include <cstdint>

namespace Configuration
{
	using FloatType = double;

	static constexpr uint32_t MAX_FRAMERATE = 144u;
	static constexpr uint32_t FIXED_FRAMERATE = 60u;

	static constexpr sf::Vector2u WINDOW_SIZE = sf::Vector2u(1920u, 1080u);

	static constexpr uint32_t PIXEL_SIZE = 1u;
};

