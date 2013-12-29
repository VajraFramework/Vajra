#ifndef ManagedChunk_h__
#define ManagedChunk_h__

#include "Vajra/Common/Pool/Chunk.h"

template <class Meat>
class ManagedChunk {
public:
	ManagedChunk (Chunk<Meat> *_chunk) {
		this->chunk = _chunk;
		this->chunk->Use ();
	}

	ManagedChunk (const ManagedChunk<Meat>& _other) {
		this->chunk = _other.chunk;
		this->chunk->Use ();
	}

	ManagedChunk& operator= (const ManagedChunk<Meat>& _other) {
		this->chunk = _other.chunk;
		this->chunk->Use ();
		return *this;
	}

	~ManagedChunk() {
		if (this->chunk != nullptr) {
			this->chunk->DisUse ();
		}
	}

	ManagedChunk () {
		this->chunk = nullptr;
	}

	inline Chunk<Meat> *GetChunk () { return this->chunk; }
	inline void SetChunk (Chunk<Meat> *_chunk) { 
		if (this->chunk != nullptr) {
			this->chunk->DisUse ();
		}
		this->chunk = _chunk; 
		this->chunk->Use ();
	}

private:
	Chunk<Meat> *chunk;		// The chunk that is managed by this ManagedChunk
};

#endif // ManagedChunk_h__

