#pragma once

#include <array>
#include <unordered_map>
#include <cassert>
#include "ecs/Entity.h"

namespace ecs {

	/// The virtual inheritance of IComponentArray is unfortunate but,
	/// we'll have a list of every ComponentArray (one per component type), 
	/// and we need to notify all of them when an entity is destroyed so that it can 
	/// remove the entity's data if it exists. The only way to keep a list of multiple 
	/// templated types is to keep a list of their common interface so that we can 
	/// call EntityDestroyed() on all of them.

	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
	};

	/// Some performance downside using unordered_maps because they are not contiguous
	/// But the unordered_maps have the nice property of supporting find(), 
	/// insert(), and delete(), which allow for asserting validity without "if(valid)" 
	/// checks and it's a bit clearer then setting array elements to some "INVALID" value.

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:

		void InsertData(Entity entity, T component);
		void RemoveData(Entity entity);
		T& GetData(Entity entity);
		void EntityDestroyed(Entity entity) override;

	private:

		// packed array
		std::array<T, MAX_ENTITIES> m_Array;

		// Map from an entity ID to an array index.
		std::unordered_map<Entity, size_t> m_EntityToIndexMap;

		// Map from an array index to an entity ID.
		std::unordered_map<size_t, Entity> m_IndexToEntityMap;

		// Total size of valid entries in the array.
		size_t m_Size;
	};

	/// ----------------------------------------------
	/// InsertData
	/// ----------------------------------------------

	template<typename T>
	inline void ComponentArray<T>::InsertData(Entity entity, T component)
	{
		assert(m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end() 
			&& "Component added to same entity more than once.");

		size_t newIndex = m_Size;
		m_EntityToIndexMap[entity] = newIndex;
		m_IndexToEntityMap[newIndex] = entity;
		m_Array[newIndex] = component;
		++m_Size;
	}

	/// ----------------------------------------------
	/// RemoveData
	/// ----------------------------------------------
	
	template<typename T>
	inline void ComponentArray<T>::RemoveData(Entity entity)
	{
		assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() 
			&& "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
		size_t indexOfLastElement = m_Size - 1;
		m_Array[indexOfRemovedEntity] = m_Array[indexOfLastElement];

		// Update map to point to moved spot
		Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
		m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		m_EntityToIndexMap.erase(entity);
		m_IndexToEntityMap.erase(indexOfLastElement);

		--m_Size;
	}

	/// ----------------------------------------------
	/// GetData
	/// ----------------------------------------------
	
	template<typename T>
	inline T& ComponentArray<T>::GetData(Entity entity)
	{
		// TODO: insert return statement here
		assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() 
			&& "Retrieving non-existent component.");

		// Return a reference to the entity's component
		return m_Array[m_EntityToIndexMap[entity]];
	}

	/// ----------------------------------------------
	/// EntityDestroyed
	/// ----------------------------------------------
	
	template<typename T>
	inline void ComponentArray<T>::EntityDestroyed(Entity entity)
	{
		if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
		{
			// Remove the entity's component if it existed
			RemoveData(entity);
		}
	}

}