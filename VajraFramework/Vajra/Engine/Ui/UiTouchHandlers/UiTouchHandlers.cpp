#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/Ui/UiCallbackComponent/UiCallbackComponent.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Engine/Ui/UiTouchHandlers/UiTouchHandlers.h"

#include "Vajra/Common/Objects/ObjectRegistry.h"

UiTouchHandlers::UiTouchHandlers() {
	this->eventForwarder = new GameObject(ENGINE->GetSceneGraphUi());
	UiCallbackComponent* uCC = this->eventForwarder->AddComponent<UiCallbackComponent>();
	uCC->SetUiTouchHandlers(this);
}

UiTouchHandlers::~UiTouchHandlers() {
	delete eventForwarder;
}

void UiTouchHandlers::HandleMessageCallback(MessageChunk /*messageChunk*/) {
	
}
