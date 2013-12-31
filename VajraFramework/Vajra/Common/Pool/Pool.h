#ifndef Pool_h__
#define Pool_h__

#include "Vajra/Common/Pool/Chunk.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#include <deque>

#if USING_MULTITHREADING_SUPPORT_FOR_POOLS
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#endif // USING_MULTITHREADING_SUPPORT_FOR_POOLS

#define POOL_SIZE_INCREMENT 1000
#define POOL_SIZE_LOW_THRESHOLD 100


// Forward Declarations:
template <class TMeat>
class ManagedChunk;

template <class TMeat>
class Pool
{
public:
	Pool();
	~Pool();

	ManagedChunk<TMeat> GetManagedChunk();
	void ReleaseChunk(Chunk<TMeat>* _chunk);
	int GetFreeCount()  { return this->chunks_free.size(); }
	int GetInuseCount() { return this->chunks_inuse; }

private:
	std::deque<Chunk<TMeat>*> chunks_free;
#if USING_MULTITHREADING_SUPPORT_FOR_POOLS
	boost::mutex chunks_free_mutex;
#endif // USING_MULTITHREADING_SUPPORT_FOR_POOLS
	int chunks_inuse;

	// Utility Functions:
	void makeMoreChunks();
};



////////////////////////////////////////////////////////////////////////////////

template <class TMeat>
Pool<TMeat>::Pool() {
	// FRAMEWORK->GetLogger()->dbglog("\nInitializing Pool ...");

	this->chunks_inuse = 0;

#if USING_MULTITHREADING_SUPPORT_FOR_POOLS
	boost::mutex::scoped_lock lock_on_chunks_free_queue (this->chunks_free_mutex);
#endif // USING_MULTITHREADING_SUPPORT_FOR_POOLS

	// Create an initial pool:
	this->makeMoreChunks();

#if USING_MULTITHREADING_SUPPORT_FOR_POOLS
	// Scoped lock on this->chunks_free_mutex falls off
#endif // USING_MULTITHREADING_SUPPORT_FOR_POOLS
}

template <class TMeat>
Pool<TMeat>::~Pool() {
	// FRAMEWORK->GetLogger()->dbglog("\nDestroying Pool ...");

	// Free the pools:
	for (auto chunk_it = this->chunks_free.begin (); chunk_it != this->chunks_free.end (); ++chunk_it) {
		Chunk<TMeat>* chunk = (*chunk_it);
		delete chunk;
	}
	this->chunks_free.clear ();
}

template <class TMeat>
ManagedChunk<TMeat> Pool<TMeat>::GetManagedChunk() {
#if USING_MULTITHREADING_SUPPORT_FOR_POOLS
	boost::mutex::scoped_lock lock_on_chunks_free_queue (this->chunks_free_mutex);
#endif // USING_MULTITHREADING_SUPPORT_FOR_POOLS

	if (this->chunks_free.size () == 0) {
		// Out of free chunks; must make more
		this->makeMoreChunks ();
	}

	// ASSERT (this->chunks_free.size () != 0, "No more chunks");
	Chunk<TMeat>* chunk = this->chunks_free.front ();
	this->chunks_free.pop_front ();

	if (this->chunks_free.size () < POOL_SIZE_LOW_THRESHOLD) {
		// Low on free chunks; must make more
		this->makeMoreChunks ();
	}

	this->chunks_inuse++;
	ManagedChunk<TMeat> mchunk(chunk);
	return (mchunk);

#if USING_MULTITHREADING_SUPPORT_FOR_POOLS
	// Scoped lock on this->chunks_free_mutex falls off
#endif // USING_MULTITHREADING_SUPPORT_FOR_POOLS
}

template <class TMeat>
void Pool<TMeat>::ReleaseChunk(Chunk<TMeat>* _chunk) {
	// FRAMEWORK->GetLogger()->dbglog("\nIn pool: Moving chunk to free list");
#if USING_MULTITHREADING_SUPPORT_FOR_POOLS
	boost::mutex::scoped_lock lock_on_chunks_free_queue (this->chunks_free_mutex);
#endif // USING_MULTITHREADING_SUPPORT_FOR_POOLS

	this->chunks_free.push_back (_chunk);
	ASSERT(this->chunks_inuse > 0, "chunks_inuse > 0");
	this->chunks_inuse--;

#if USING_MULTITHREADING_SUPPORT_FOR_POOLS
	// Scoped lock on this->chunks_free_mutex falls off
#endif // USING_MULTITHREADING_SUPPORT_FOR_POOLS
}

template <class TMeat>
void Pool<TMeat>::makeMoreChunks() {
	FRAMEWORK->GetLogger()->dbglog("\nLow on chunks, making %d more", POOL_SIZE_INCREMENT);

	// Create a pool of size POOL_SIZE_INCREMENT and add them all to the free list:
	for (int i = 0; i < POOL_SIZE_INCREMENT; i++) {
		TMeat* meat = new TMeat();
		Chunk<TMeat>* chunk = new Chunk<TMeat>();
		chunk->SetMeat(meat);
		chunk->SetPool(this);
		this->chunks_free.push_back(chunk);
	}
}

#endif // Pool_h__

