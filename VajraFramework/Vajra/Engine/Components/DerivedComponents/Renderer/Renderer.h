#ifndef RENDERER_H
#define RENDERER_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/AssetLibrary/Assets/MeshAssets/MeshAsset.h"

#include <memory>
#include <string>

// Forward Declarations:
class Object;
class Mesh;

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

private:
	void init();
	void destroy();

	static unsigned int componentTypeId;

	std::string shaderName;
};

#endif // RENDERER_H
