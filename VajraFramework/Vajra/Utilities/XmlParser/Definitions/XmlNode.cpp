#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/XmlParser/Definitions/XmlNode.h"

XmlNode::XmlNode() {
	this->init();
}

XmlNode::~XmlNode() {
	this->destroy();
}

void XmlNode::Print() {
	FRAMEWORK->GetLogger()->dbglog("\nNode name: %s", this->name.c_str());
	FRAMEWORK->GetLogger()->dbglog("\n\tAttributes: ");
	for (XmlAttribute* attribute : this->attributes) {
		attribute->Print();
	}
	if (this->value != "") {
		FRAMEWORK->GetLogger()->dbglog("\n\tValue: %s", this->value.c_str());
	}
}

void XmlNode::init() {
	this->parentRef = nullptr;
}

void XmlNode::destroy() {
	// TODO [Implement] Free up allocated members
}

void XmlNode::addAttribute(XmlAttribute* attribute) {
	this->attributes.push_back(attribute);
}

void XmlNode::addChild(XmlNode* childNode) {
	this->children.push_back(childNode);
}

void XmlNode::setParent(XmlNode* parentNode) {
	this->parentRef = parentNode;
}
