#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/XmlParser/Definitions/XmlAttribute.h"

XmlAttribute::XmlAttribute() {
	this->init();
}

XmlAttribute::~XmlAttribute() {
	this->destroy();
}

void XmlAttribute::Print() {
	FRAMEWORK->GetLogger()->dbglog("\n%s : %s", this->name.c_str(), this->value.c_str());
}

void XmlAttribute::SetName(std::string name_) {
	this->name = name_;
}

void XmlAttribute::SetValue(std::string value_) {
	// TODO [Cleanup] Move this to StringUtilities
	while (value_.find_first_of('"') != std::string::npos) {
		value_ = value_.erase(value_.find_first_of('"'), 1);
	}
	this->value = value_;
}

void XmlAttribute::init() {
}

void XmlAttribute::destroy() {
}
