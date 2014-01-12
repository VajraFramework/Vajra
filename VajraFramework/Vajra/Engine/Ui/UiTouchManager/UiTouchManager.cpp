#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Engine/Ui/UiTouchManager/UiTouchManager.h"

#include <algorithm>

UiTouchManager::UiTouchManager() {
	this->init();
}

UiTouchManager::~UiTouchManager() {
	this->destroy();
}

void UiTouchManager::init() {
	this->currentlyRecievingUiObjectId = OBJECT_ID_INVALID;
}

void UiTouchManager::destroy() {
}

void UiTouchManager::RegisterTouchableUiElement(ObjectIdType id) {
#ifdef DEBUG
	ASSERT(std::find(this->registeredUiObjects.begin(), this->registeredUiObjects.end(), id) == this->registeredUiObjects.end(), "Not duplicate UiElement being registered again for touch");
#endif
	this->registeredUiObjects.push_back(id);
}

void UiTouchManager::UnRegisterTouchableUiElement(ObjectIdType id) {
	auto foundRegisterdId_it = std::find(this->registeredUiObjects.begin(), this->registeredUiObjects.end(), id);
	ASSERT(foundRegisterdId_it != this->registeredUiObjects.end(), "Found object id to unregister from touch");
	if (foundRegisterdId_it != this->registeredUiObjects.end()) {
		this->registeredUiObjects.erase(foundRegisterdId_it);
	}
}

bool UiTouchManager::TestTouchCapture(Touch touch) {

	// Check if any registered clickable UiObjects got touched:
	for (ObjectIdType uiObjectId : this->registeredUiObjects) {
		// TODO [Implement] Ensure type safety here
		UiObject* uiObject = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(uiObjectId);
		if (uiObject->IsPointWithin(touch.pos.x, touch.pos.y)) {
			this->currentlyRecievingUiObjectId = uiObjectId;
			break;
		}
	}

	if (this->currentlyRecievingUiObjectId != OBJECT_ID_INVALID) {
		// Forward the touch to the Object which will forward it to its click handlers
		UiObject* uiObject = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->currentlyRecievingUiObjectId);
		ASSERT(uiObject != nullptr, "Currently receiving uiObject not null");
		uiObject->OnClick(touch);
		return true;
	}
	return false;
}

void UiTouchManager::OnTouchUpdate(int touchIndex) {
	Touch touch = ENGINE->GetInput()->GetTouch(touchIndex);

	// Forward the touch to the currently recieving Object which will forward it to its touch move handlers
	if (this->currentlyRecievingUiObjectId != OBJECT_ID_INVALID) {
		Touch touch = ENGINE->GetInput()->GetTouch(touchIndex);
		// Forward the touch to the Object which will forward it to its touch move handlers
		UiObject* uiObject = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->currentlyRecievingUiObjectId);
		ASSERT(uiObject != nullptr, "Currently receiving uiObject not null");
		uiObject->OnTouchMove(touch);
	}

	if (touch.phase == TouchPhase::Ended || touch.phase == TouchPhase::Cancelled) {
		this->currentlyRecievingUiObjectId = OBJECT_ID_INVALID;
	}
}
