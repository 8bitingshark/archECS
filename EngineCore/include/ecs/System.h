#pragma once
#include "ecs/Entity.h"
#include <set>

namespace ecs {

	using SystemType = std::uint8_t;

	class System {
		
	public:

		std::set<Entity> m_Entities;

		/*
		// Usage example:

		for (auto const& entity : mEntities)
		{
			auto& rigidBody = GetComponent<RigidBody>(entity);
			auto& transform = GetComponent<Transform>(entity);
			auto const& gravity = GetComponent<Gravity>(entity);

			transform.position += rigidBody.velocity * dt;

			rigidBody.velocity += gravity.force * dt;
		}
		*/


	};
}