#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Grid/GridRoom.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/GameConstants/GameConstants.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Utilities/MathUtilities.h"

#define DEFAULT_GAME_CAM_OFFSET glm::vec3(0.0f, 25.0f, 0.0f)
void shadyCameraTweenCallback(ObjectIdType gameObjectId, std::string /* tweenClipName */) {
	GameObject* camObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if(camObj != nullptr) {
		ShadyCamera* shadyCam = camObj->GetComponent<ShadyCamera>();
		ASSERT(shadyCam != nullptr, "GameObject has a ShadyCamera component");
		if(shadyCam != nullptr) {
			shadyCam->isMoving = false;
		}
	}	
}

ShadyCamera::ShadyCamera() : Camera() {
	this->init();
}

ShadyCamera::ShadyCamera(Object* object_) : Camera(object_) {
	this->init();
}

ShadyCamera::~ShadyCamera() {
	this->destroy();
}

void ShadyCamera::init() {
	// Set camera properties
	this->SetFOV(30.0f inRadians);
	
	this->camSpeed = GetFloatGameConstant(GAME_CONSTANT_camSpeed);
	this->zoomSpeed = GetFloatGameConstant(GAME_CONSTANT_zoomSpeed);
	this->maxZoomSpeed = GetFloatGameConstant(GAME_CONSTANT_maxZoomSpeed);
	this->maxWrongZoomAmt = GetFloatGameConstant(GAME_CONSTANT_maxWrongZoomAmt);
	this->changeModeThreshold = GetFloatGameConstant(GAME_CONSTANT_changeModeThreshold);
	
	this->camMode = CameraMode::CameraMode_Game;
	this->isMoving = false;

	this->gameObjectRef = (GameObject*)this->GetObject();
	ASSERT(this->gameObjectRef->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");

	Transform* camTransform = this->gameObjectRef->GetTransform();

	// TODO [Implement] : set this based on device resolution
	this->gameCamOffset = DEFAULT_GAME_CAM_OFFSET;

	// TODO [Implement] : get overview pos and start room from level data
	this->overviewPos = glm::vec3(0.0f, 50.0f, 0.0f);
	this->gameCamPos = this->gameCamOffset;

	camTransform->SetPosition(this->gameCamPos);
	camTransform->Rotate(90.0f inRadians, XAXIS);
	camTransform->Rotate(180.0f inRadians, camTransform->GetForward());
}

void ShadyCamera::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
	this->gameObjectRef = nullptr;
}

void ShadyCamera::loadCameraData(GridCell* startUnitCell, glm::vec3 overviewPos, glm::vec3 startPos, bool useStartPos) {

	this->overviewPos = overviewPos;
	Transform* camTransform = this->gameObjectRef->GetTransform();
	if(useStartPos) {
		camTransform->SetPosition(startPos);
		this->MoveGameCamToRoom(startUnitCell->x, startUnitCell->z);
	} else {
		this->setCurrentRoomCenter(SINGLETONS->GetGridManager()->GetGrid()->GetRoomCenter(startUnitCell->x, startUnitCell->z));
		this->gameCamPos = this->currentRoomCenter + this->gameCamOffset;		
		camTransform->SetPosition(gameCamPos);

	}
	// We only want to subscribe to messages once we set up the inital camera data 
	this->addSubscriptionToMessageType(MESSAGE_TYPE_PINCH_GESTURE, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SELECTED_UNIT_CHANGED, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ROOM_ENTERED, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_UNIT_SPECIAL_HIT, this->GetTypeId(), false);
}

void ShadyCamera::HandleMessage(MessageChunk messageChunk) {
	Camera::HandleMessage(messageChunk);
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_PINCH_GESTURE:
			this->onPinch();
			break;
		case MESSAGE_TYPE_SELECTED_UNIT_CHANGED:
			this->MoveGameCamToRoom(messageChunk->messageData.iv1.x, messageChunk->messageData.iv1.z);
			break;
		case MESSAGE_TYPE_GRID_ROOM_ENTERED:
			if(messageChunk->messageData.iv1.x == SINGLETONS->GetGridManager()->GetSelectedUnitId()) {
				this->MoveGameCamToRoom(messageChunk->messageData.iv1.x, messageChunk->messageData.iv1.z);
			}
			break;
		case MESSAGE_TYPE_UNIT_SPECIAL_HIT:
			if(messageChunk->GetSenderId() == SINGLETONS->GetGridManager()->GetSelectedUnitId()) {
				this->MoveGameCamToRoom(messageChunk->messageData.iv1.x, messageChunk->messageData.iv1.z);
			}
			break;
		default:
			break;
	}
}

void ShadyCamera::MoveTo(glm::vec3 newPos) {
	this->moveTo_internal(newPos, this->camSpeed);
}

void ShadyCamera::MoveTo(float x, float y, float z) {
	this->MoveTo(glm::vec3(x, y, z));
}

void ShadyCamera::PanTo(float x, float z) {
	float y = this->gameObjectRef->GetTransform()->GetPosition().y;
	this->MoveTo(glm::vec3(x, y, z));
}

void ShadyCamera::ZoomTo(float y) {
	glm::vec3 newPos = this->gameObjectRef->GetTransform()->GetPosition();
	newPos.y = y;
	this->gameObjectRef->GetTransform()->SetPosition(newPos);
}

void ShadyCamera::ZoomBy(float yOffset) {
	glm::vec3 newPos = this->gameObjectRef->GetTransform()->GetPosition();
	newPos.y += yOffset;
	this->gameObjectRef->GetTransform()->SetPosition(newPos);
}

void ShadyCamera::MoveToGamePos() {
	this->MoveTo(this->gameCamPos);
}

void ShadyCamera::moveTo_internal(glm::vec3 newPos, float speed) {
	glm::vec3 curPos = this->gameObjectRef->GetTransform()->GetPosition();
	float dist = glm::distance(curPos, newPos);
	if (dist > ROUNDING_ERROR) {
		this->isMoving = true;
		ENGINE->GetTween()->TweenPosition(this->gameObjectRef->GetId(), curPos, newPos,
				dist / speed, true, TWEEN_TRANSLATION_CURVE_TYPE_LINEAR, false, shadyCameraTweenCallback);
	}
}

// TODO [Implement] : if needed
void ShadyCamera::LevelStartPan() {

}

void ShadyCamera::FollowGameObjectDirectly(ObjectIdType unitId) {
	if(this->camMode == CameraMode::CameraMode_Game) {
		GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(unitId);
		if(go != nullptr) {
			this->setCurrentCameraHeight(go->GetTransform()->GetPosition().y);
			this->updateGameCamPos();
		}
	}
	
}

void ShadyCamera::MoveGameCamToRoom(int i, int j) {
	GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(i, j);
	glm::vec3 roomCenter = SINGLETONS->GetGridManager()->GetGrid()->GetRoomCenter(cell);
	this->setCurrentRoomCenter(roomCenter);
	this->setCurrentCameraHeight(SINGLETONS->GetGridManager()->GetGrid()->ConvertElevationToWorldY(cell->y));
	this->updateGameCamPos();
}

void ShadyCamera::setCurrentRoomCenter(glm::vec3 roomCenter) {
	if(roomCenter != ZERO_VEC3) {
		this->currentRoomCenter = roomCenter;
	}
}

void ShadyCamera::setCurrentCameraHeight(float elevatorInWorldUnits) {
	glm::vec3 newOffset = DEFAULT_GAME_CAM_OFFSET + glm::vec3(0.0f, elevatorInWorldUnits, 0.0f);
	FRAMEWORK->GetLogger()->dbglog("\nsetCurrentCameraHeight %f" , elevatorInWorldUnits);
	if(this->gameCamOffset != newOffset) {
		this->gameCamOffset = newOffset;
	}
}
void ShadyCamera::updateGameCamPos() {
	glm::vec3 newPos = this->currentRoomCenter + this->gameCamOffset;
	if(newPos != this->gameCamPos) {
		this->gameCamPos = newPos;
		if(this->camMode == CameraMode::CameraMode_Game) {
			this->MoveToGamePos();
		}
	}
}

void ShadyCamera::onPinch() {
	this->isMoving = true;
	float pinchVel = ENGINE->GetInput()->GetPinch().velocity;
	float zoomAmt = -pinchVel;
	clamp(zoomAmt, -this->maxZoomSpeed, this->maxZoomSpeed);

	// Zoom the camera by the clamped pinch
	this->ZoomBy(zoomAmt);

	// Clamp the camera zoom in the "wrong" direction 
	glm::vec3 curPos = this->gameObjectRef->GetTransform()->GetPosition();
	float zoomDela = 0;

	if(this->camMode == CameraMode::CameraMode_Game) {
		zoomDela = std::abs(this->gameCamPos.y - curPos.y);	
		// if the camera is zooming in
		if(curPos.y < this->gameCamPos.y && zoomDela > this->maxWrongZoomAmt) {
			this->ZoomBy(zoomDela - this->maxWrongZoomAmt);
		}
	} else if(this->camMode == CameraMode::CameraMode_Overview) {
		zoomDela = std::abs(this->overviewPos.y - curPos.y);	
		// if the cam is zooming out
		if(curPos.y > this->overviewPos.y && zoomDela > this->maxWrongZoomAmt) {
			this->ZoomBy(-1 * (zoomDela - this->maxWrongZoomAmt)); 
		}
	}
	
	// Once the pinch ends devices where to zoom the camera to
	if(ENGINE->GetInput()->HasPinchEnded()) {
		this->isMoving = false;
		if(this->camMode == CameraMode::CameraMode_Game) {	
			if(curPos.y - this->changeModeThreshold <= this->gameCamPos.y) {
				this->moveTo_internal(this->gameCamPos, this->camSpeed);
				this->setCameraMode(CameraMode::CameraMode_Game);
			} else {
				this->moveTo_internal(this->overviewPos, this->zoomSpeed);
				this->setCameraMode(CameraMode::CameraMode_Overview);
			}
		} else if(this->camMode == CameraMode::CameraMode_Overview) {
			if(curPos.y + this->changeModeThreshold <= this->overviewPos.y) {
				this->moveTo_internal(this->gameCamPos, this->zoomSpeed);
				this->setCameraMode(CameraMode::CameraMode_Game);
			} else {
				this->moveTo_internal(this->overviewPos, this->camSpeed);
				this->setCameraMode(CameraMode::CameraMode_Overview);
			}
		}
	}
}

void ShadyCamera::setCameraMode(CameraMode newMode) {
	if(newMode != this->camMode) {
		this->camMode = newMode;

		ENGINE->GetMessageHub()->SendMulticastMessage(MESSAGE_TYPE_CAMERA_MODE_CHANGED, this->gameObjectRef->GetId());
	}
}

void ShadyCamera::onUnitChanged() {
	//this->MoveToRoom();
}
