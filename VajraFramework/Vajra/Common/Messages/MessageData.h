#ifndef MESSAGE_DATA_H
#define MESSAGE_DATA_H

#include "Vajra/Common/Messages/Declarations.h"

class MessageData {
public:
	MessageData();
	virtual ~MessageData();

	inline MessageDataType GetMessageDataType() { return this->messageDataType; }

protected:

	MessageDataType messageDataType;

private:
	// Data fields in Derived message types go here
};

#endif // MESSAGE_DATA_H
