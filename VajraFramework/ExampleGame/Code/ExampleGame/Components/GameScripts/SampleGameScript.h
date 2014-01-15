//
//  SampleGameScript.h
//  ExampleGame
//
//  Created by Alex Hogue on 12/22/13.
//  Copyright (c) 2013 Vajra. All rights reserved.
//

#ifndef SAMPLE_GAMESCRIPT_H
#define SAMPLE_GAMESCRIPT_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/GameObject/GameObject.h"

// Forward Declarations:
class Transform;

class SampleGameScript : public Component {
public:
	SampleGameScript();
	SampleGameScript(Object* object_);
	~SampleGameScript();

	static inline unsigned int GetTypeId() { return componentTypeId; }

	virtual void start();
	virtual void end();
	virtual void update();

private:
	void init();
	void destroy();

	// Utility functions:
	Transform* getTransform() { return this->gameObjectRef->GetTransform(); }

	GameObject* gameObjectRef;

	static unsigned int componentTypeId;
};

#endif // SAMPLE_GAMESCRIPT_H
