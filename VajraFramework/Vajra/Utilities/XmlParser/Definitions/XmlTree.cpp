#include "Vajra/Utilities/XmlParser/Definitions/XmlNode.h"
#include "Vajra/Utilities/XmlParser/Definitions/XmlTree.h"

XmlTree::XmlTree() {
	this->init();
}

XmlTree::~XmlTree() {
	this->destroy();
}

void XmlTree::Print() {
	if (this->rootNode != nullptr) {
		this->print_node(this->rootNode);
	}
}

void XmlTree::print_node(XmlNode* xmlNode) {
	xmlNode->Print();

	for (XmlNode* childNode : xmlNode->children) {
		this->print_node(childNode);
	}
}

void XmlTree::init() {
	this->rootNode = nullptr;
}

void XmlTree::destroy() {
	if (this->rootNode != nullptr) {
		delete this->rootNode;
	}
}
