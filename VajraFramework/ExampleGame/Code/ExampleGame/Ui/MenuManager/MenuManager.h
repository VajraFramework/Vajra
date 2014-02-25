#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Vajra/Common/Components/Component.h"

//[[COMPONENT]]//
class MenuManager : public Component {	
public:
	MenuManager();
	MenuManager(Object* object_);
	~MenuManager();

	void HandleMessage(MessageChunk messageChunk);

private:
	void init();
	void destroy();

	static ComponentIdType componentTypeId;
};

#endif //MENU_MANAGER_H
