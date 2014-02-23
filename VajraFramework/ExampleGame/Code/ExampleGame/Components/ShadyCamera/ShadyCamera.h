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
class GridRoom;

//[[COMPONENT]]//
class ShadyCamera : public Camera {
public:
	enum CameraMode {
		CameraMode_Game,
		CameraMode_Overview,
	};

	ShadyCamera();
	ShadyCamera(Object* object_);
	~ShadyCamera();

	static inline unsigned int GetTypeId() { return Camera::GetTypeId(); }

	virtual void HandleMessage(MessageChunk messageChunk);

	// General camera moving functions
	void MoveTo(glm::vec3 newPos);
	//[[PROPERTY]]//
	void MoveTo(float x, float y, float z);

	// Moves the camera along x and z, keeping y (height) the same
	void PanTo(float x, float z);
	void ZoomTo(float y);
	void ZoomBy(float yOffset);

	void MoveGameCamToRoom(int i, int j); // Cell indicies
	// Move camera to game room
	void MoveToGamePos(); // move to current room
	// Sets current room and then moves to it

	// Pan the camera along a preset path for a level intro
	void LevelStartPan();

	void FollowGameObjectDirectly(ObjectIdType unitId);

	inline CameraMode GetCameraMode() {return this->camMode;}
	inline bool IsMoving() { return this->isMoving; }
private:
	void init();
	void destroy();

	void loadCameraData(GridCell* startUnitCell, glm::vec3 overviewPos, glm::vec3 startPos, bool useStartPos);

	GameObject* gameObjectRef;

	CameraMode camMode;
	bool isMoving;
	// Find and store the current position for the game camera
	void setCurrentRoomCenter(glm::vec3 roomCenter); // center of the room
	void setCurrentCameraHeight(float /*elevatorInWorldUnits*/); 
	void updateGameCamPos();

	bool newPinch;
	float velocityThreshold;
	float heightThreshold;
	void onPinch();
	void setCameraMode(CameraMode newMode);
	void onUnitChanged();

	void moveTo_internal(glm::vec3 pos, float speed);

	// consts
	float camSpeed;
	float zoomSpeed;
	float maxZoomSpeed;
	float maxWrongZoomAmt;
	float changeModeThreshold;

	glm::vec3 gameCamOffset;

	glm::vec3 gameCamPos;
	glm::vec3 overviewPos;

	glm::vec3 currentRoomCenter;

	friend class LevelLoader;
	friend void shadyCameraTweenCallback(ObjectIdType gameObjectId, std::string /* tweenClipName */);
};

#endif // SHADY_CAMERA_H
