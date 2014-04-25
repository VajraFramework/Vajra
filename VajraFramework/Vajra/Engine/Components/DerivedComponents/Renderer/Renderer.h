#ifndef RENDERER_H
#define RENDERER_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/AssetLibrary/Assets/MeshAssets/MeshAsset.h"

#include <memory>
#include <string>

// Forward Declarations:
class Object;
class Mesh;

// Not exposing this as a Component that can be added via XML since it is purely virtual
class Renderer : public Component {
public:
	Renderer();
	Renderer(Object* object_);
	virtual ~Renderer();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk) = 0;

	virtual void Draw() = 0;

	inline std::string GetShaderName() { return this->shaderName; }
	inline void SetShaderName(std::string shaderName_) { this->shaderName = shaderName_; }

	inline bool HasTransperancy()                         { return this->hasTransperancy;             }
	inline bool IsOverlay()                               { return this->isOverlay;                   }
	inline void SetIsOverlay      (bool isOverlay_)       { this->isOverlay = isOverlay_;             }

	inline bool IsStatic() { return this->isStatic; }
	inline void SetIsStatic(bool isStatic_) { this->isStatic = isStatic_; }

	inline bool IsRenderBatch() { return this->isRenderBatch; }
	inline void SetIsRenderBatch(bool isRenderBatch_) { this->isRenderBatch = isRenderBatch_; }

	inline bool ShouldPreventCulling()                     { return this->preventCulling;             }
	inline void SetPreventCulling   (bool preventCulling_) { this->preventCulling = preventCulling_;  }

protected:
	void addToRenderList();
	void removeFromRenderList();

	inline void setHasTransperancy(bool hasTransperancy_) { this->hasTransperancy = hasTransperancy_; }

private:
	void init();
	void destroy();

	static unsigned int componentTypeId;

	std::string shaderName;
	bool hasTransperancy;
	bool isOverlay;

	bool preventCulling;

	bool isStatic;

	bool isRenderBatch;
};

#endif // RENDERER_H
