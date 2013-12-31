#ifndef _Chunk_h__
#define _Chunk_h__

// Forward Declarations:
template <class TMeat>
class Pool;

template <class TMeat>
class Chunk {
public:
	TMeat *meat;		// Pointer to the actual little bit of data whose pool is being managed

	Chunk();

	inline void SetPool(Pool<TMeat> *_pool)	{ this->pool = _pool; }
	inline void SetMeat(TMeat *_meat)		{ this->meat = _meat; }
	inline void Use()                       { this->use_count++; }
	inline void DisUse() {
		int reducedUseCount = --this->use_count;
		if (reducedUseCount <= 0) {
			this->returnToPool();
		}
	}

	inline int GetUseCount()		        { return this->use_count; }

private:
	int use_count;
	Pool<TMeat> *pool;

	// Utility Functions:
	void returnToPool();
};



////////////////////////////////////////////////////////////////////////////////

template <class TMeat>
Chunk<TMeat>::Chunk() {
	this->meat = nullptr;
	this->pool = nullptr;
	this->use_count = 0;
}

// #include "Vajra/Common/Pool/Pool.h"
template <class TMeat>
void Chunk<TMeat>::returnToPool() {
	if (this->pool != nullptr) {
		this->pool->ReleaseChunk(this);
	} else {
		// No pool to go back to. Kill self:
		delete this;
	}
}



#endif // _Chunk_h__

