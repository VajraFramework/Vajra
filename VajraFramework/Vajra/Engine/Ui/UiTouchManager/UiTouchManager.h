#ifndef UITOUCHMANAGER_H
#define UITOUCHMANAGER_H

#include <vector>

class UiTouchManager {
public:
	UiTouchManager();
	virtual ~UiTouchManager();

	void RegisterTouchableUiElement(ObjectIdType id);
	void UnRegisterTouchableUiElement(ObjectIdType id);

	bool TestTouchCapture(Touch touch);
	void OnTouchUpdate(int touchIndex);

private:
	void init();
	void destroy();

	ObjectIdType currentlyRecievingUiObjectId;
	std::vector<ObjectIdType> registeredUiObjects;
};

#endif // UITOUCHMANAGER_H
