#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Engine/Lighting/ShadowMapUpdateListener.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

ComponentIdType ShadowMapUpdateListener::componentTypeId = COMPONENT_TYPE_ID_SHADOW_MAP_UPDATE_LISTENER;

ShadowMapUpdateListener::ShadowMapUpdateListener() : Component() {
	this->init();
}

ShadowMapUpdateListener::ShadowMapUpdateListener(Object* object_)  : Component(object_) {
	this->init();
}

ShadowMapUpdateListener::~ShadowMapUpdateListener() {
	this->destroy();
}

void ShadowMapUpdateListener::HandleMessage(MessageChunk messageChunk) {
	switch (messageChunk->GetMessageType()) {
	default: {
	case MESSAGE_TYPE_DIRECTIONAL_LIGHT_CHANGED: {
		ObjectIdType id = messageChunk->messageData.iv1.x;
		DirectionalLight* mainLight = ENGINE->GetSceneGraph3D()->GetMainDirectionalLight();
		if (mainLight != nullptr && mainLight->GetObject()->GetId() == id) {
			ENGINE->GetShadowMap()->mainDirectionalLightChanged();
		}
	} break;

	case MESSAGE_TYPE_CAMERA_CHANGED: {
		ObjectIdType id = messageChunk->messageData.iv1.x;
		Camera* camera = ENGINE->GetSceneGraph3D()->GetMainCamera();
		if (camera != nullptr && camera->GetObject()->GetId() == id) {
			ENGINE->GetShadowMap()->mainCameraChanged();
		}
	} break;

	} break;
	}
}

void ShadowMapUpdateListener::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_DIRECTIONAL_LIGHT_CHANGED, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_CAMERA_CHANGED, this->GetTypeId(), false);
}

void ShadowMapUpdateListener::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
