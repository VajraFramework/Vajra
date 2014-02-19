#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
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

	UiObject* temp_currentlyReceivingUiObject = nullptr;
	/*
	 * Check if any registered clickable UiObjects got touched:
	 * However, multiple over-lapping ui objects might have gotten touched,
	 * Deliver the touch to the ui object with the highest z
	 */
	for (ObjectIdType uiObjectId : this->registeredUiObjects) {
		UiObject* uiObject = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(uiObjectId);
		ASSERT(uiObject != nullptr, "Ui Object of id %d found", uiObjectId);
		ASSERT(uiObject->GetClassType() & CLASS_TYPE_UIOBJECT, "Object is a ui object");
		if (uiObject->IsPointWithin(touch.pos.x, touch.pos.y)) {
			if (temp_currentlyReceivingUiObject == nullptr ||
				temp_currentlyReceivingUiObject->GetTransform()->GetPosition().z < uiObject->GetTransform()->GetPosition().z) {

				temp_currentlyReceivingUiObject = uiObject;
				this->currentlyRecievingUiObjectId = uiObjectId;
			}
		}
	}

	if (this->currentlyRecievingUiObjectId != OBJECT_ID_INVALID) {
		// Forward the touch to the Object which will forward it to its click handlers
		UiObject* uiObject = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->currentlyRecievingUiObjectId);
		ASSERT(uiObject != nullptr, "Currently receiving uiObject not null");
		uiObject->OnTouchDown(touch);
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

		if (uiObject->IsPointWithin(touch.pos.x, touch.pos.y)) {
			if (touch.phase == TouchPhase::Ended || touch.phase == TouchPhase::Cancelled) {
				uiObject->OnTouchUp(touch);
			}
			else {
				uiObject->OnTouchMove(touch);
			}
		}
		else {
			this->currentlyRecievingUiObjectId = OBJECT_ID_INVALID;
		}
	}

	if (touch.phase == TouchPhase::Ended || touch.phase == TouchPhase::Cancelled) {
		this->currentlyRecievingUiObjectId = OBJECT_ID_INVALID;
	}
}
