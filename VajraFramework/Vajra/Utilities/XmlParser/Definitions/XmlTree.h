#ifndef XML_TREE_H
#define XML_TREE_H

// Forward Declarations:
class XmlNode;

class XmlTree {
public:
	XmlTree();
	~XmlTree();

	void Print();

	XmlNode* GetRootNode() { return this->rootNode; }

private:
	void init();
	void destroy();

	// Utility Functions:
	void print_node(XmlNode* xmlNode);

	XmlNode* rootNode;

	friend class XmlParser;
};

#endif // XML_TREE_H
