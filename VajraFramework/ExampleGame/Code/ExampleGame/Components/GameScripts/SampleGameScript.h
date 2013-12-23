//
//  SampleGameScript.h
//  ExampleGame
//
//  Created by Alex Hogue on 12/22/13.
//  Copyright (c) 2013 Vajra. All rights reserved.
//

#ifndef SAMPLE_GAMESCRIPT_H
#define SAMPLE_GAMESCRIPT_H

#include "Vajra/Engine/Components/DerivedComponents/GameScript/GameScript.h"

class SampleGameScript : public GameScript {
public:
	SampleGameScript();
	SampleGameScript(Object* object_);
	~SampleGameScript();

	virtual void start();
	virtual void end();
	virtual void update();
};

#endif // SAMPLE_GAMESCRIPT_H
