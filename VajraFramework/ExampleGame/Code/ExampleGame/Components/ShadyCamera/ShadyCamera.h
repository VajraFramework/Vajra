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
#include "Libraries/glm/glm.hpp"

class GridCell;
class GridManager;

class ShadyCamera : public Camera {
public:
	enum Camera_Mode {
		CAMERA_MODE_GAME,
		CAMERA_MODE_OVERVIEW
	};

	ShadyCamera();
	ShadyCamera(Object* object_);
	~ShadyCamera();

	virtual void HandleMessage(Message* message);

	void setGridManager(GridManager* value);

	// General camera moving functions
	void MoveTo(glm::vec3 newPos);
	void MoveTo(float x, float y, float z);

	// Moves the camera along x and z, keeping y (height) the same
	void PanTo(float x, float z);
	void ZoomTo(float y);
	void ZoomBy(float yOffset);
	
	// Given a cell or arbitary coords pan the camera to the room
	void PanToRoom(float x, float z);
	void PanToRoom(GridCell cell);

	// Zooming between game and overview mode
	void ZoomToGame(); // zoom back to previous room
	void ZoomToGame(GridCell cell); // cell is a cell in the current room

	void ZoomToOverview();

	// Pan the camera along a preset path for a level intro
	void LevelStartPan();

private:
	void init();
	void destroy();

	GridManager* gridManager;

	Camera_Mode camMode = Camera_Mode::CAMERA_MODE_GAME;

	float camHeight;
	glm::vec3 overviewPos;
};

#endif // SHADY_CAMERA_H
