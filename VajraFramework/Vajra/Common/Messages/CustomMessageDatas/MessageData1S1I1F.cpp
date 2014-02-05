#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I1F.h"

MessageData1S1I1F::MessageData1S1I1F() : MessageData() {
	this->messageDataType = MESSAGEDATA_TYPE_1S_1I_1F;
	this->init();
}

MessageData1S1I1F::~MessageData1S1I1F() {
	this->destroy();
}

void MessageData1S1I1F::init() {
	this->i = -1;
	this->f = 0.0f;
}

void MessageData1S1I1F::destroy() {
}
