#include <cassert>
#include "soa\CtmSmallObjAllocator.h"

/// -----------------------------------------------------------------------------
/// CtmSmallObjAllocator::CtmSmallObjAllocator
/// ctor
/// -----------------------------------------------------------------------------

soa::CtmSmallObjAllocator::CtmSmallObjAllocator(std::size_t chunkSize, std::size_t maxObjectSize)
	: m_chunkSize(chunkSize), m_maxObjSize(maxObjectSize)
{
}

/// -----------------------------------------------------------------------------
/// Functor
/// -----------------------------------------------------------------------------

namespace {

	struct CompareCtmFixedAllocatorSize {

		bool operator()(const soa::CtmFixedAllocator& x, std::size_t numBytes) const noexcept {
			return x.GetBlockSize() < numBytes;
		}
	};
}

/// -----------------------------------------------------------------------------
/// CtmSmallObjAllocator::Allocate
/// -----------------------------------------------------------------------------

void* soa::CtmSmallObjAllocator::Allocate(std::size_t numBytes)
{
	if (numBytes > m_maxObjSize)
	{
		return std::malloc(numBytes); // previous: return operator new(numBytes); Bad with global overrides
	}


	if (m_pLastAlloc && m_pLastAlloc->GetBlockSize() == numBytes)
	{
		return m_pLastAlloc->Allocate();
	}

	std::vector<CtmFixedAllocator>::iterator it = std::lower_bound(
		m_Pool.begin(),
		m_Pool.end(),
		numBytes,
		CompareCtmFixedAllocatorSize{});

	if (it == m_Pool.end() || it->GetBlockSize() != numBytes)
	{
		it = m_Pool.insert(it, CtmFixedAllocator(numBytes));
		m_pLastDealloc = &*m_Pool.begin();
	}

	m_pLastAlloc = &*it;

	return m_pLastAlloc->Allocate();
}

/// -----------------------------------------------------------------------------
/// CtmSmallObjAllocator::Deallocate
/// -----------------------------------------------------------------------------

void soa::CtmSmallObjAllocator::Deallocate(void* p, std::size_t numBytes)
{
	if (numBytes > m_maxObjSize)
	{
		return std::free(p);
	}

	if (m_pLastDealloc && m_pLastDealloc->GetBlockSize() == numBytes)
	{
		m_pLastDealloc->Deallocate(p);
		return;
	}

	std::vector<CtmFixedAllocator>::iterator it = std::lower_bound(
		m_Pool.begin(),
		m_Pool.end(),
		numBytes,
		CompareCtmFixedAllocatorSize{});

	assert(it != m_Pool.end());
	assert(it->GetBlockSize() == numBytes);

	m_pLastDealloc = &*it;

	m_pLastDealloc->Deallocate(p);
}