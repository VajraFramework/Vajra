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

	void ZoomTo(float y);
	void ZoomBy(float yOffset);

	void MoveGameCamToRoom(int i, int j, int elevation = -1); // Cell indicies
	void MoveToGamePos(); // move to current room

	void FollowGameObjectDirectly(ObjectIdType unitId);

	inline CameraMode GetCameraMode() {return this->camMode;}
	inline bool IsMoving() { return this->isMoving; }

	void setCameraMode(CameraMode newMode);

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
	void onUnitChanged();

	void moveTo_internal_overTime(glm::vec3 pos, float time);
	
	// consts
	float panTime;
	float gameToOverviewTime;
	float heightChangeTime;

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
