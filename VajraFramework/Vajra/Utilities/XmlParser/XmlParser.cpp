#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/FileUtilities.h"
#include "Vajra/Utilities/XmlParser/XmlParser.h"
#include "Vajra/Utilities/StringUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include <sstream>

XmlParser::XmlParser() {
	this->init();
}

XmlParser::~XmlParser() {
	this->destroy();
}

void XmlParser::ParseXmlFile(std::string filePath) {
	FRAMEWORK->GetLogger()->dbglog("\nParsing xml file: %s", filePath.c_str());

	this->xmlTree = new XmlTree();

	std::ifstream file(filePath.c_str());
	ASSERT(file.good(), "Successfully opened xml file at path %s for reading", filePath.c_str());
	this->parseXmlFile_internal(file);
}

void XmlParser::init() {
	this->xmlTree = nullptr;
}

void XmlParser::destroy() {
	if (this->xmlTree != nullptr) {
		delete this->xmlTree;
	}
}

void XmlParser::parseXmlFile_internal(std::ifstream& file) {
	while (file.good()) {
		AdvanceFileSeekOverWhiteSpace(file);
		this->parseXmlNode(file);
		AdvanceFileSeekOverWhiteSpace(file);
	}

	if (!this->parseStack.empty()) {
		FRAMEWORK->GetLogger()->dbglog("\nParse stack was not empty at the end of parsing. Printing left over contents of parse stack:");
		while (!this->parseStack.empty()) {
			XmlNode* xmlNode = this->parseStack.top();
			this->parseStack.pop();
			FRAMEWORK->GetLogger()->dbglog("\n%s", xmlNode->GetName().c_str());
		}
		ASSERT(0, "Parse stack empty");
	}
}

bool endOfNodeTagReached(std::ifstream& file) {
	// Checks if "</" is about to be read.
	// Does not advance the file's seek position.
	char nextChar;
	nextChar = file.peek();
	if (nextChar != '<') {
		return false;
	}

	file.get(nextChar);
	nextChar = file.peek();
	file.seekg(-1, file.cur);
	if (nextChar == '/') {
		return true;
	}
	return false;
}


void XmlParser::Print() {
	if (this->xmlTree != nullptr) {
		this->xmlTree->Print();
	}
}

void XmlParser::parseAttributes(std::string& attributesString, XmlNode* xmlNode) {
	if (attributesString == "") {
		return;
	}
	std::vector<std::string> attributes = StringUtilities::SplitStringIntoTokensOnDelimiter(attributesString, ' ');
	for (std::string attribute : attributes) {
		std::vector<std::string> name_value_pair = StringUtilities::SplitStringIntoTokensOnDelimiter(attribute, '=');
		ASSERT(name_value_pair.size() == 2, "Properly formed attribute name=value pairs should have size 2, found %u, in attributesData: %s", name_value_pair.size(), attributesString.c_str());
		//
		XmlAttribute* xmlAttribute = new XmlAttribute();
		xmlAttribute->SetName (name_value_pair[0]);
		xmlAttribute->SetValue(name_value_pair[1]);
		//
		xmlNode->addAttribute(xmlAttribute);
	}
}

void XmlParser::parseXmlNode(std::ifstream& file) {
	// Look for the beginning of an xml node:
	AdvanceFileSeekTillChar(file, '<');
	std::string beginNewNode = ReadFileTillChar(file, '>', false);
	std::size_t spacePosition = beginNewNode.find_first_of(' ');
	std::string nodeName = beginNewNode.substr(0, spacePosition);
	std::string attributesData;
	if (spacePosition != std::string::npos) {
		attributesData = beginNewNode.substr(spacePosition, std::string::npos);
	}

	XmlNode* parentXmlNode = nullptr;
	if (!this->parseStack.empty()) {
		parentXmlNode = this->parseStack.top();
	}

	// Create a new xml node and add it to the parse stack and the tree:
	XmlNode* xmlNode = new XmlNode();
	xmlNode->name = nodeName;
	//
	this->parseStack.push(xmlNode);
	if (parentXmlNode != nullptr) {
		parentXmlNode->addChild(xmlNode);
		xmlNode->setParent(parentXmlNode);
	} else {
		this->xmlTree->rootNode = xmlNode;
	}

	// Parse attributes for this node:
	if (attributesData != "") {
		this->parseAttributes(attributesData, xmlNode);
	}

	FRAMEWORK->GetLogger()->dbglog("\nReading xml node: %s", xmlNode->GetName().c_str());
	// FRAMEWORK->GetLogger()->dbglog("\nNode attributes: %s", attributesData.c_str());

	AdvanceFileSeekOverWhiteSpace(file);
	while (!endOfNodeTagReached(file)) {
		bool nodeHasNestedNode = false;
		bool nodeHasValue = false;
		//
		char nextChar = file.peek();
		if (nextChar != '<') {
			nodeHasValue = true;
		} else{
			// Peek one more character:
			file.get(nextChar);
			nextChar = file.peek();
			if (nextChar != '/') {
				nodeHasNestedNode = true;
			}
			file.seekg(-1, file.cur);
		}
		//
		if (nodeHasNestedNode) {
			this->parseXmlNode(file);

		} else if (nodeHasValue) {
			std::string nodeValue = ReadFileTillBeforeChar(file, '<', false);
			xmlNode->value = nodeValue;
			// FRAMEWORK->GetLogger()->dbglog("\nNode (%s) has value: %s", xmlNode->GetName().c_str(), xmlNode->value.c_str());
		}
		AdvanceFileSeekOverWhiteSpace(file);
	}

	// At this point we should be just behind the opening chevron of the node ending tag:
	AdvanceFileSeekOverWhiteSpace(file);
	char nextChar;
	file.get(nextChar);
	ASSERT(nextChar == '<', "Unexpected character: %c, expected <", nextChar);
	file.get(nextChar);
	ASSERT(nextChar == '/', "Unexpected character: %c, expected /", nextChar);
	//
	std::string endingNodeName;
	endingNodeName = ReadFileTillChar(file, '>', true);
	ASSERT(this->parseStack.top()->name == endingNodeName, "Ending node tag name (%s) matches current top of parsing stack (%s)", endingNodeName.c_str(), this->parseStack.top()->name.c_str());
	this->parseStack.pop();
}
