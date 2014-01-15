#ifndef XMLNODE_H
#define XMLNODE_H

#include "Vajra/Utilities/XmlParser/Definitions/XmlAttribute.h"

#include <string>
#include <vector>

class XmlNode {
public:
	XmlNode();
	~XmlNode();

	std::string GetName()  { return this->name;  }
	std::string GetValue() { return this->value; }

	void Print();

private:
	void init();
	void destroy();

	void addAttribute(XmlAttribute* attribute);
	void addChild(XmlNode* childNode);
	void setParent(XmlNode* parentNode);

	std::string name;
	std::string value;

	std::vector<XmlAttribute*> attributes;

	XmlNode* parentRef;
	std::vector<XmlNode*> children;

	friend class XmlParser;
	friend class XmlTree;
};

#endif // XMLNODE_H
