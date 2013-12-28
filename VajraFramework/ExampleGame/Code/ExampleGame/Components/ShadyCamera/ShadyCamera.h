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

	// Move camera to game room
	void MoveToRoom(); // zoom back to previous room
	void MoveToRoom(float x, float z);
	void MoveToRoom(GridCell* cell); // cell is a cell in the current room

	void MoveToOverview();

	// Pan the camera along a preset path for a level intro
	void LevelStartPan();

private:
	void init();
	void destroy();

	GameObject* gameObject;
	GridManager* gridManager;

	Camera_Mode camMode = Camera_Mode::CAMERA_MODE_GAME;
	// Find and store the current position for the game camera
	void SetGameCam(float x, float z);


	float gameCamHeight;
	glm::vec3 gameCamPos;
	glm::vec3 overviewPos;
};

#endif // SHADY_CAMERA_H
