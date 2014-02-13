#ifndef UI_CALLBACK_COMPONENT_H
#define UI_CALLBACK_COMPONENT_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/MessageHub/Declarations.h"


// Not exposing this as a Component that can be added via XML
class UiTouchHandlers;
class UiCallbackComponent : public Component {
public:
	UiCallbackComponent();
	UiCallbackComponent(Object* object_);
	~UiCallbackComponent();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	static inline ComponentIdType GetTypeId()  { return componentTypeId; }

	void SetUiTouchHandlers(UiTouchHandlers* handlers) { this->touchHandlers = handlers; }
	void SubscribeToMessage(MessageType /*messageType*/);
private:
	void init();
	void destroy();

	UiTouchHandlers* touchHandlers;
	void* uiTouchHandlersOnHandleMessage;

	static ComponentIdType componentTypeId;
};

#endif //UI_CALLBACK_COMPONENT_H