#ifndef TUTORIAL_CACHE_H
#define TUTORIAL_CACHE_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/AssetLibrary/Assets/TextureAssets/TextureAsset.h"

#include <memory>

// Forward Declarations:
class GameObject;

//[[COMPONENT]]//
class TutorialCache : public Component {
public:
	TutorialCache();
	TutorialCache(Object* object_);
	~TutorialCache();

	static inline ComponentIdType GetTypeId()  { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

private:
	void init();
	void destroy();

	void preloadImage(std::string imageName);
	void purgeCache();

	static ComponentIdType componentTypeId;

	std::shared_ptr<TextureAsset> cachedTutorialImageAsset;
};


#endif // TUTORIAL_CACHE_H
