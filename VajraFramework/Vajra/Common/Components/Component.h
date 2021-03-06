#ifndef COMPONENT_H
#define COMPONENT_H

#include "Vajra/Common/Messages/Declarations.h"
#include "Vajra/Engine/Components/ComponentTypes/ComponentTypeIds.h"
#include "Vajra/Engine/MessageHub/Declarations.h"
#include "Vajra/Utilities/Utilities.h"

#include <string>
#include <vector>

// Forward Declarations:
class Object;
class Message;

class Component {
public:
	Component();
	Component(Object* object_);
	virtual ~Component();

	virtual void HandleMessage(MessageChunk messageChunk);

	inline Object* GetObject() { return this->object; }
	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	inline void SetObject(Object* object_) { this->object = object_; }

protected:
	void addSubscriptionToMessageType(MessageType messageType, ComponentIdType selfComponentIdType, bool onLocalObject);
	void removeSubscriptionToMessageType(MessageType messageType, ComponentIdType selfComponentIdType);
	void removeSubscriptionToAllMessageTypes(ComponentIdType selfComponentIdType);

	virtual void start()  {}
	virtual void update() {}
	virtual void end()    {}

private:
	void init(Object* object_ = 0);
	void destroy();

	static ComponentIdType componentTypeId;

	Object* object;

	std::vector<MessageType> broadcastMessageTypesSubscribedTo;
};

#endif // COMPONENT_H
