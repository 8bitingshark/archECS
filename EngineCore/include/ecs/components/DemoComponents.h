#pragma once
#include "IMesh.h"
#include <DirectXMath.h>

namespace ecs {

	struct Transform {
		DirectX::XMFLOAT3 position{ 0, 0, 0 };
		DirectX::XMFLOAT3 rotation{ 0, 0, 0 };
		DirectX::XMFLOAT3 scale{ 1, 1, 1 };
	};

	struct Velocity {
		float angularSpeed = 1.0f; // rad/s per la rotazione circolare
		float radius = 2.0f;       // distanza dal centro
	};

	struct Color {
		DirectX::XMFLOAT4 value{ 1, 1, 1, 1 };
	};

	struct MeshComponent {
		std::shared_ptr<gfx::IMesh> mesh;
	};

}