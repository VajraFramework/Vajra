#ifndef GAMEUI_TOUCH_HANDLERS_H
#define GAMEUI_TOUCH_HANDLERS_H

#include "Vajra/Engine/Ui/UiTouchHandlers/UiTouchHandlers.h"

#include <vector>

struct TutorialData {
public:
	bool hasFired;
	MessageType msgType;
	glm::vec3 vector3Data;
};

class GameUiTouchHandlers : public UiTouchHandlers {
public:
	GameUiTouchHandlers();
	// @Override
	virtual void OnTouchDownHandlers(UiObject* uiObject, Touch touch);
	virtual void OnTouchMoveHandlers(UiObject* uiObject, Touch touch);
	virtual void OnTouchUpHandlers  (UiObject* uiObject, Touch touch);

	// @Override
	virtual void HandleMessageCallback(MessageChunk messageChunk);
private:
	void setupTutorial(std::string levelName);
	void tryTutorial(int index, MessageChunk messageChunk);
	std::vector<TutorialData> tutorials;
	bool isTutorialLevel;
};

#endif // GAMEUI_TOUCH_HANDLERS_H
