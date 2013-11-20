#ifndef MESSAGE_DATA_1S1I3FV_H
#define MESSAGE_DATA_1S1I3FV_H

#include "Vajra/Common/Messages/MessageData.h"
#include "Vajra/Utilities/MathUtilities.h"

#include <string>

class MessageData1S1I3FV : public MessageData {
public:
	MessageData1S1I3FV();
	~MessageData1S1I3FV();

	std::string s;
	int i;
	glm::vec3 fv1;
	glm::vec3 fv2;
	glm::vec3 fv3;

private:
	void init();
	void destroy();
};

#endif // MESSAGE_DATA_1S1I3FV_H
