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
	void SetDecalImages(std::string gameMode_image, float gameMode_imageSize, std::string overviewMode_image, float overviewMode_imageSize);

protected:
	// @Override
	virtual void start();
	virtual void update();

private:
	void init();
	void destroy();

	void handleCameraModeChanged();
	void updatePosition();

	GameObject* gameObjectRef;
	GameObject* decalObjectRef;

	float scales[2];

	unsigned int currentMode;

	static ComponentIdType componentTypeId;
};

#endif // DECAL_H
