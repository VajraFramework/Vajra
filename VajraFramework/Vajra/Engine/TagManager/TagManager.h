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

	std::map<std::string, unsigned int> stringToBitmask;

	friend class Engine;
	friend class GameObject;
};

#endif // TAGMANAGER_H
