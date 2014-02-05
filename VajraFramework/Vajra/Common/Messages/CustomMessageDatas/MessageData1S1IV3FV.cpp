#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1IV3FV.h"

MessageData1S1IV3FV::MessageData1S1IV3FV() : MessageData() {
	this->messageDataType = MESSAGEDATA_TYPE_1S_1I_3FV;
	this->init();
}

MessageData1S1IV3FV::~MessageData1S1IV3FV() {
	this->destroy();
}

void MessageData1S1IV3FV::init() {
	this->iv1 = glm::ivec3(-1, -1, -1);
}

void MessageData1S1IV3FV::destroy() {
}
