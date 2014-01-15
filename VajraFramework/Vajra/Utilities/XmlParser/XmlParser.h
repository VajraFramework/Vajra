#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "Vajra/Utilities/XmlParser/Definitions/XmlNode.h"
#include "Vajra/Utilities/XmlParser/Definitions/XmlTree.h"

#include <fstream>
#include <stack>
#include <string>


class XmlParser {
public:
	XmlParser();
	~XmlParser();

	void ParseXmlFile(std::string filePath);
	void Print();

private:
	void init();
	void destroy();

	// Utility Functions:
	void parseXmlFile_internal(std::ifstream& file);
	void parseXmlNode(std::ifstream& file);
	void parseAttributes(std::string& attributesString, XmlNode* xmlNode);

	XmlTree* xmlTree;

	std::stack<XmlNode*> parseStack;
};

#endif // XML_PARSER_H
