#pragma once
#include <queue>
#include <array>
#include <bitset>
#include <cstdint>
#include "ecs/Entity.h"
#include "ecs/Component.h"

namespace ecs {

	/// The Entity Manager is in charge of distributing entity IDs 
	/// and keeping record of which IDs are in use and which are not.

	class EntityManager {

	public:

		EntityManager();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		void SetSignature(Entity entity, std::bitset<MAX_COMPONENTS> in_Signature);
		std::bitset<MAX_COMPONENTS> GetSignature(Entity entity) const;

	private:
		std::queue<Entity> m_AvailableEntities{};

		// tracks down which components an entity has
		// A system would also register its interest in certain components 
		// as another signature. Then it’s a simple bitwise comparison to 
		// ensure that an entity’s signature contains the system’s signature.
		std::array<std::bitset<MAX_COMPONENTS>, MAX_ENTITIES> m_Signatures{};
		std::uint32_t m_LivingEntityCount = 0;
	};
}