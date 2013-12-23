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

// forward declarations
class GameObject;
class MeshRenderer;
class Transform;

class GameScript : public Component {
public:
	GameScript();
	GameScript(Object* object_);
	~GameScript();

	static inline ComponentIdType GetTypeId() { return componentTypeId; };

protected:
	virtual void HandleMessage(Message* message);

	// Virtual Methods
	virtual void start()  {}
	virtual void end()    {}
	virtual void update() {}

	// Useful accessors for game-scripting
	//GameObject* gameObject;
	GameObject* gameObject;
	Transform* getTransform();
	MeshRenderer* getMeshRenderer();

private:
	void init();
	void destroy();

	Transform* transform;
	MeshRenderer* meshRenderer;

	static unsigned int componentTypeId;
};
#endif // GAMESCRIPT_H
