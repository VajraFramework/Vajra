//
//  Input.h
//  ogles
//
//  Created by Alex Hogue on 12/2/13.
//  Copyright (c) 2013 vajra. All rights reserved.
//

#ifndef __ogles__Input__
#define __ogles__Input__

#include "Vajra/Common/Objects/Object.h"

#include <iostream>

class Input : public Object {
public:
	~Input();

private:
	Input();
	void init();
	void destroy();

	void updateInput();

	friend class Engine;
};

#endif /* defined(__ogles__Input__) */
