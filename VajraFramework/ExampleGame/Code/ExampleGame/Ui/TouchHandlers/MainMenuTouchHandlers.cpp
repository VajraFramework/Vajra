#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"


void MainMenuTouchHandlers::OnTouchDownHandlers(UiObject* uiObject, Touch touch) {
	FRAMEWORK->GetLogger()->dbglog("\nGot touch down: name (%s): %f, %f", uiObject->GetUiObjectName().c_str(), touch.pos.x, touch.pos.y);

	if (uiObject->GetUiObjectName() == "testButton") {
		// Do something

	} else {
		// Do something

	}

}

void MainMenuTouchHandlers::OnTouchMoveHandlers(UiObject* uiObject, Touch touch) {
	FRAMEWORK->GetLogger()->dbglog("\nGot touch move: name (%s): %f, %f", uiObject->GetUiObjectName().c_str(), touch.pos.x, touch.pos.y);

	if (uiObject->GetUiObjectName() == "testButton") {
		// Do something

	} else {
		// Do something

	}

}

void MainMenuTouchHandlers::OnTouchUpHandlers(UiObject* uiObject, Touch touch) {
	FRAMEWORK->GetLogger()->dbglog("\nGot touch up  : name (%s): %f, %f", uiObject->GetUiObjectName().c_str(), touch.pos.x, touch.pos.y);

	if (uiObject->GetUiObjectName() == "testButton") {
		// Do something

	} else {
		// Do something

	}

}
