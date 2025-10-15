#pragma once
#include <cstdint>

namespace ecs {

	// A simple type alias
	using ComponentType = std::uint8_t;

	// Used to define the size of arrays later on
	const ComponentType MAX_COMPONENTS = 32;
}