#ifndef TAGMANAGER_H
#define TAGMANAGER_H

#include "Vajra/Common/Objects/Object.h"

#include <map>

class TagManager : public Object {	
public:
	~TagManager();


	void LoadTagsFromFile(std::string tagFile);
private:
	TagManager();
	void init();
	void destroy();

	std::map<unsigned char, std::string> bitMaskToString;
	std::map<std::string, unsigned char> stringToBitMask;

	friend class Engine;
};

#endif // TAGMANAGER_H
