#ifndef DECAL_H
#define DECAL_H

#include "Vajra/Common/Components/Component.h"

// Forward Declarations:
class GameObject;

//[[COMPONENT]]//
class Decal : public Component {
public:
	Decal();
	Decal(Object* object_);
	~Decal();

	static inline ComponentIdType GetTypeId()  { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	void SetDecalImages(std::string gameMode_image, std::string overviewMode_image);

protected:

private:
	void init();
	void destroy();

	void handleCameraModeChanged();

	GameObject* gameObjectRef;

	GameObject* decalObjectRef;

	static ComponentIdType componentTypeId;
};

#endif // DECAL_H
