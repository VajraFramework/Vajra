#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I3FV.h"

MessageData1S1I3FV::MessageData1S1I3FV() : MessageData() {
	this->messageDataType = MESSAGEDATA_TYPE_1S_1I_3FV;
	this->init();
}

MessageData1S1I3FV::~MessageData1S1I3FV() {
	this->destroy();
}

void MessageData1S1I3FV::init() {
	this->i = -1;
}

void MessageData1S1I3FV::destroy() {
}
