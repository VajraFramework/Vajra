#ifndef XML_ATTRIBUTE_H
#define XML_ATTRIBUTE_H

#include <string>

class XmlAttribute {
public:
	XmlAttribute();
	~XmlAttribute();

	void Print();

	void SetName(std::string name_);
	void SetValue(std::string attribute_);

private:
	void init();
	void destroy();

	std::string name;
	std::string value;
};

#endif // XML_ATTRIBUTE_H
