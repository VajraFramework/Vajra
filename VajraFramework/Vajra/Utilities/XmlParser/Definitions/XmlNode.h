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

	std::vector<XmlNode*> GetChildren() const { return this->children; }
	std::vector<XmlNode*> GetChildrenByNodeName(std::string nodeName) const;

	XmlNode* GetFirstChildByNodeName (std::string nodeName = "") const;
	XmlNode* GetNextSiblingByNodeName(std::string nodeName = "") const;

	std::vector<std::string> GetAttributeNames() const;

	std::string GetAttributeValueS(std::string attributeName) const;
	int         GetAttributeValueI(std::string attributeName) const;
	float       GetAttributeValueF(std::string attributeName) const;
	bool        GetAttributeValueB(std::string attributeName) const;

	void Print();

private:
	void init();
	void destroy();

	void addAttribute(XmlAttribute* attribute);
	void addChild(XmlNode* childNode);
	void setParent(XmlNode* parentNode);

	std::string getAttributeValueAsString(std::string& attributeName) const;

	std::string name;
	std::string value;

	std::vector<XmlAttribute*> attributes;

	XmlNode* parentRef;
	std::vector<XmlNode*> children;

	friend class XmlParser;
	friend class XmlTree;
};

#endif // XMLNODE_H
