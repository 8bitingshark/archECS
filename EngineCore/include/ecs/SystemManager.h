#pragma once

#include <memory>
#include <bitset>
#include <unordered_map>
#include <cassert>
#include "System.h"
#include "Entity.h"
#include "Component.h"

namespace ecs {

	// -----------------------------------------
	// Compile-time System Type IDs
	// -----------------------------------------

	inline SystemType GetNextSystemTypeID() {
		static SystemType lastID = 0;
		return lastID++;
	}

	template<typename T>
	SystemType GetSystemTypeID() {
		static SystemType typeID = GetNextSystemTypeID();
		return typeID;
	}

	// -----------------------------------------
	// System Manager
	// -----------------------------------------

	class SystemManager {

	public:

		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			SystemType type = ecs::GetSystemTypeID<T>();

			assert(m_Systems.find(type) == m_Systems.end() 
				&& "System already registered!");

			auto system = std::make_shared<T>();
			
			m_Systems.insert({ type, system });

			return system;
		}

		template<typename T>
		void SetSignature(std::bitset<MAX_COMPONENTS> signature)
		{
			SystemType type = ecs::GetSystemTypeID<T>();

			assert(m_Systems.find(type) != m_Systems.end() 
				&& "System not registered!");

			m_Signatures[type] = signature;
		}

		void EntityDestroyed(Entity entity)
		{
			for (auto const& [type, system] : m_Systems)
				system->m_Entities.erase(entity);
		}

		void EntitySignatureChanged(Entity entity, std::bitset<MAX_COMPONENTS> entitySignature)
		{
			for (auto const& [type, system] : m_Systems)
			{
				auto const& systemSignature = m_Signatures[type];

				if ((entitySignature & systemSignature) == systemSignature)
					system->m_Entities.insert(entity);
				else
					system->m_Entities.erase(entity);
			}
		}

	private:
		std::unordered_map<SystemType, std::bitset<MAX_COMPONENTS>> m_Signatures{};
		std::unordered_map<SystemType, std::shared_ptr<System>> m_Systems{};
	};
}