#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/StringUtilities.h"
#include "Vajra/Utilities/Utilities.h"
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

std::vector<XmlNode*> XmlNode::GetChildrenByNodeName(std::string nodeName) const {
	std::vector<XmlNode*> result;
	for (XmlNode* childNode : this->children) {
		if (childNode->name == nodeName) {
			result.push_back(childNode);
		}
	}
	return result;
}

XmlNode* XmlNode::GetFirstChildByNodeName(std::string nodeName /* = "" */) const {
	for (XmlNode* childNode : this->children) {
		if (nodeName == "" || childNode->name == nodeName) {
			return childNode;
		}
	}
	return nullptr;
}

XmlNode* XmlNode::GetNextSiblingByNodeName(std::string nodeName /* = "" */) const {
	if (this->parentRef == nullptr) {
		return nullptr;
	}

	bool foundSelfInParentsChildren = false;
	for (XmlNode* sibling : this->parentRef->children) {
		if (sibling == this) {
			foundSelfInParentsChildren = true;
		}
		if (foundSelfInParentsChildren) {
			if (nodeName == "" || sibling->name == nodeName) {
				return sibling;
			}
		}
	}
	return nullptr;
}

std::vector<std::string> XmlNode::GetAttributeNames() const {
	std::vector<std::string> result;
	for (XmlAttribute* attribute : this->attributes) {
		result.push_back(attribute->name);
	}
	return result;
}

std::string XmlNode::GetAttributeValueS(std::string attributeName) const {
	return this->getAttributeValueAsString(attributeName);
}

int XmlNode::GetAttributeValueI(std::string attributeName) const {
	std::string valueAsString = this->getAttributeValueAsString(attributeName);
	return StringUtilities::ConvertStringToInt(valueAsString);
}

float XmlNode::GetAttributeValueF(std::string attributeName) const {
	std::string valueAsString = this->getAttributeValueAsString(attributeName);
	return StringUtilities::ConvertStringToFloat(valueAsString);
}

bool XmlNode::GetAttributeValueB(std::string attributeName) const {
	std::string valueAsString = this->getAttributeValueAsString(attributeName);
	return StringUtilities::ConvertStringToBool(valueAsString);
}

std::string XmlNode::getAttributeValueAsString(std::string& attributeName) const {
	for (XmlAttribute* attribute : this->attributes) {
		if (attribute->name == attributeName) {
			return attribute->value;
		}
	}
	ASSERT(0, "Attribute by name %s was found in node %s", attributeName.c_str(), this->name.c_str());
	return "";
}
