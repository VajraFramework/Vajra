//
//  GameScript.h
//  VajraStaticLibrary
//
//  Created by Alex Hogue on 12/13/13.
//  Copyright (c) 2013 Vajra. All rights reserved.
//

#ifndef GAMESCRIPT_H
#define GAMESCRIPT_H

#include "Vajra/Common/Components/Component.h"

class GameScript : public Component {
public:
	GameScript();
	GameScript(Object* object_);
	~GameScript();

	static inline ComponentIdType GetTypeId() { return componentTypeId; };

	// Virtual Methods
	virtual void start()  = 0;
	virtual void end()    = 0;
	virtual void update() = 0;
protected:
	virtual void HandleMessage(Message* message);
private:

	static unsigned int componentTypeId;
};
#endif // GAMESCRIPT_H
