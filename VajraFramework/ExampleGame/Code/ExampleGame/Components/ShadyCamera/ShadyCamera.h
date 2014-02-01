//
//  ShadyCamera.h
//  ExampleGame
//
//  Created by Alex Hogue on 12/23/13.
//  Copyright (c) 2013 Vajra. All rights reserved.
//

#ifndef SHADY_CAMERA_H
#define SHADY_CAMERA_H

#include "Libraries/glm/glm.hpp"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/GameObject/GameObject.h"

class GridCell;
class GridManager;

//[[COMPONENT]]//
class ShadyCamera : public Camera {
public:
	enum CameraMode {
		CameraMode_Game,
		CameraMode_Overview
	};

	ShadyCamera();
	ShadyCamera(Object* object_);
	~ShadyCamera();

	static inline unsigned int GetTypeId() { return Camera::GetTypeId(); }

	virtual void HandleMessage(MessageChunk messageChunk);

	void SetGridManager(GridManager* value);

	// General camera moving functions
	void MoveTo(glm::vec3 newPos);
	//[[PROPERTY]]//
	void MoveTo(float x, float y, float z);

	// Moves the camera along x and z, keeping y (height) the same
	void PanTo(float x, float z);
	void ZoomTo(float y);
	void ZoomBy(float yOffset);

	// Move camera to game room
	void MoveToCurrentRoom(); // move to current room
	// Sets current room and then moves to it
	//[[PROPERTY]]//
	void MoveToRoom(float x, float z); // location in current room
	void MoveToRoom(GridCell* cell); // cell is a cell in the current room

	//[[PROPERTY]]//
	void MoveToOverview();

	// Pan the camera along a preset path for a level intro
	void LevelStartPan();

protected:
	void update();

private:
	void init();
	void destroy();

	GameObject* gameObjectRef;
	GridManager* gridManagerRef;

	CameraMode camMode;
	// Find and store the current position for the game camera
	void setGameCameraPosition(float x, float z);

	bool newPinch;
	float velocityThreshold;
	float heightThreshold;
	void onPinch();
	bool tryModeSwitch(float velocity);
	void setCameraMode(CameraMode newMode);

	float camSpeed;
	float gameCamHeight;
	glm::vec3 gameCamPos;
	glm::vec3 overviewPos;
};

#endif // SHADY_CAMERA_H
