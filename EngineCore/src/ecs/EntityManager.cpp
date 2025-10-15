#include "ecs/EntityManager.h"
#include <cassert>

/// ----------------------------------------------------------------
/// EntityManager Ctor
/// ----------------------------------------------------------------

ecs::EntityManager::EntityManager()
{
	// init the queue with all possible ideas
	for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
	{
		m_AvailableEntities.push(entity);
	}
}

/// ----------------------------------------------------------------
/// EntityManager::CreateEntity
/// ----------------------------------------------------------------

ecs::Entity ecs::EntityManager::CreateEntity()
{
	Entity id = m_AvailableEntities.front();
	m_AvailableEntities.pop();
	++m_LivingEntityCount;
	return id;
}

/// ----------------------------------------------------------------
/// EntityManager::DestroyEntity
/// ----------------------------------------------------------------

void ecs::EntityManager::DestroyEntity(Entity entity)
{
	assert(entity < MAX_ENTITIES && "Entity out of range.");

	m_Signatures[entity].reset();
	m_AvailableEntities.push(entity);

	--m_LivingEntityCount;
}

/// ----------------------------------------------------------------
/// EntityManager::SetSignature
/// ----------------------------------------------------------------

void ecs::EntityManager::SetSignature(Entity entity, std::bitset<MAX_COMPONENTS> in_Signature)
{
	assert(entity < MAX_ENTITIES && "Entity out of range.");
	m_Signatures[entity] = in_Signature;
}

/// ----------------------------------------------------------------
/// EntityManager::GetSignature
/// ----------------------------------------------------------------

std::bitset<ecs::MAX_COMPONENTS> ecs::EntityManager::GetSignature(Entity entity) const
{
	assert(entity < MAX_ENTITIES && "Entity out of range.");

	return m_Signatures[entity];
}
