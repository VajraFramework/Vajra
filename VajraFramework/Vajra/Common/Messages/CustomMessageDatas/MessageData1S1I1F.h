#ifndef MESSAGE_DATA_1S1I1F_H
#define MESSAGE_DATA_1S1I1F_H

#include "Vajra/Common/Messages/MessageData.h"
#include "Vajra/Utilities/MathUtilities.h"

#include <string>

class MessageData1S1I1F : public MessageData {
public:
	MessageData1S1I1F();
	~MessageData1S1I1F();

	std::string s;
	int i;
	float f;

private:
	void init();
	void destroy();
};

#endif // MESSAGE_DATA_1S1I1F_H
