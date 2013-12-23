//
//  ShadyCamera.h
//  ExampleGame
//
//  Created by Alex Hogue on 12/23/13.
//  Copyright (c) 2013 Vajra. All rights reserved.
//

#ifndef SHADY_CAMERA_H
#define SHADY_CAMERA_H

#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"

class ShadyCamera : public Camera {
public:
	ShadyCamera();
	ShadyCamera(Object* object_);
	~ShadyCamera();

private:
	void init();
	void destroy();
};

#endif // SHADY_CAMERA_H
