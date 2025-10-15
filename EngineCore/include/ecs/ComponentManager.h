#pragma once

#include <memory>
#include "ecs/Component.h"
#include "ecs/ComponentArray.h"

namespace ecs {

	// -----------------------------------------
	// Compile-time Component Type IDs
	// -----------------------------------------

	// limited to std::uint8_t
	// enough for a demo

	inline ComponentType GetNextComponentTypeID()
	{
		static ComponentType lastID = 0;
		return lastID++;
	}

	// compile-time association type+ID

	template<typename T>
	ComponentType GetComponentTypeID()
	{
		static ComponentType typeID = GetNextComponentTypeID();
		return typeID;
	}

	// In general a better solution will be using a 
	// Reflection system 

	// -----------------------------------------
	// Component Manager
	// -----------------------------------------

	class ComponentManager {

	public:

		template<typename T>
		void RegisterComponent()
		{
			const ComponentType type = ecs::GetComponentTypeID<T>();
			
			assert(m_ComponentArrays.find(type) == m_ComponentArrays.end() 
				&& "Component already registered!");

			// m_ComponentArrays[type] = std::make_shared<ComponentArray<T>>();
			m_ComponentArrays.insert({ type, std::make_shared<ComponentArray<T>>() });
		}

		template<typename T>
		void AddComponent(Entity entity, const T& component)
		{
			GetArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			GetArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return GetArray<T>()->GetData(entity);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return GetComponentTypeID<T>();
		}

		void EntityDestroyed(Entity entity)
		{
			for (auto const& [type, array] : m_ComponentArrays)
				array->EntityDestroyed(entity);
		}

	private:

		std::unordered_map<ComponentType, std::shared_ptr<IComponentArray>> m_ComponentArrays{};

		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetArray()
		{
			ComponentType type = ecs::GetComponentTypeID<T>();
			
			assert(m_ComponentArrays.find(type) != m_ComponentArrays.end() 
				&& "Component not registered!");
			
			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[type]);
		}
	};
}

/*

namespace ecs {

	/// Maybe it will be better to use Events or Reflection here
	/// instead of using typeid(T).name() as a key for component types.
	///
	/// This key is used to map component types and also to keep track
	/// to the different ComponentArrays.
	///
	/// There is a ComponentType variable that increments by one with every component
	/// type that is registered. The registration approach is one of the first method
	/// The downside is that any time you add a new type of component to your game
	/// and want to use it, you will first need to call RegisterComponent.

	class ComponentManager {

	public:

		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			assert(m_ComponentTypes.find(typeName) == m_ComponentTypes.end()
				&& "Registering component type more than once.");

			// Add this component type to the component type map
			m_ComponentTypes.insert({ typeName, m_NextComponentType });

			// Create a ComponentArray pointer and add it to the component arrays map
			m_ComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

			// Increment the value so that the next component registered will be different
			++m_NextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();

			assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end()
				&& "Component not registered before use.");

			// Return this component's type - used for creating signatures
			return m_ComponentTypes[typeName];
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			// Add a component to the array for an entity
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			// Remove a component from the array for an entity
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			// Get a reference to a component from the array for an entity
			return GetComponentArray<T>()->GetData(entity);
		}

		void EntityDestroyed(Entity entity)
		{
			// Notify each component array that an entity has been destroyed
			// If it has a component for that entity, it will remove it
			for (auto const& pair : m_ComponentArrays)
			{
				auto const& component = pair.second;

				component->EntityDestroyed(entity);
			}
		}

	private:
		// Map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> m_ComponentTypes{};

		// Map from type string pointer to a component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays{};

		// The component type to be assigned to the next registered component - starting at 0
		ComponentType m_NextComponentType{};

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
		}

	};
}
*/