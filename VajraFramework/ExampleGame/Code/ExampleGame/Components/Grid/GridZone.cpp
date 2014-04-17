//
//  GridZone.cpp
//  Created by Matt Kaufmann on 01/07/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridConstants.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridZone.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Libraries/glm/gtx/vector_angle.hpp"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

unsigned int GridZone::componentTypeId = COMPONENT_TYPE_ID_GRID_ZONE;

GridZone::GridZone() : Component() {
	this->init();
}

GridZone::GridZone(Object* object_) : Component(object_) {
	this->init();
}

GridZone::~GridZone() {
	this->destroy();
}

void GridZone::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {

	case MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT:
		this->onTransformChanged();
		break;

	case MESSAGE_TYPE_CAMERA_MODE_CHANGED: {
		this->handleCameraModeChanged();
	} break;

	default: {
	} break;
	}
}

void GridZone::GetZoneBounds(int& west, int& east, int& south, int& north) {
	west  = this->trueWestBound;
	east  = this->trueEastBound;
	south = this->trueSouthBound;
	north = this->trueNorthBound;
}

void GridZone::SetZoneBounds(int xMin, int zMin, int xMax, int zMax) {
	if (xMin <= xMax) {
		this->relativeWestBound = xMin;
		this->relativeEastBound = xMax;
	}
	else {
		this->relativeWestBound = xMax;
		this->relativeEastBound = xMin;
	}
	if (zMin <= zMax) {
		this->relativeSouthBound = zMin;
		this->relativeNorthBound = zMax;
	}
	else {
		this->relativeSouthBound = zMax;
		this->relativeNorthBound = zMin;
	}

	this->sendCellEvents();
}

bool GridZone::IsCellWithinZone(GridCell* cell) {
	if (cell != nullptr) {
		int west, east, south, north;

		this->GetZoneBounds(west, east, south, north);

		return ((cell->x >= west) && (cell->x <= east) && (cell->z >= south) && (cell->z <= north));
	}

	return false;
}

bool GridZone::updateCenterPoint() {
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	// Check if the object's center has moved to a different cell.
	int newCenterX, newCenterZ;
	SINGLETONS->GetGridManager()->GetGrid()->GetCoordinates(newCenterX, newCenterZ, trans->GetPositionWorld());
	if ((newCenterX != this->centerX) || (newCenterZ != this->centerZ)) {
		this->centerX = newCenterX;
		this->centerZ = newCenterZ;
		return true;
	}
	return false;
}

bool GridZone::updateFacing() {
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	glm::vec3 newFacing = QuaternionForwardVector(trans->GetOrientationWorld());
	// Convert the forward vector into a cardinal direction.
	if (abs(newFacing.z) >= abs(newFacing.x)) {
		if (newFacing.z >= 0.0f) {
			newFacing = ZAXIS;
		}
		else {
			newFacing = -ZAXIS;
		}
	}
	else {
		if (newFacing.x >= 0.0f) {
			newFacing = XAXIS;
		}
		else {
			newFacing = -XAXIS;
		}
	}

	// Check if the object's facing has changed.
	// Don't use glm's vector angle function because it never returns an angle greater than PI/2
	float angle = acos(glm::clamp(glm::dot(this->facing, newFacing), -1.0f, 1.0f));
	if (angle >= PI / 4.0f) {
		this->facing = newFacing;
		return true;
	}
	return false;
}

void GridZone::updateZoneBounds() {
	if (this->updateCenterPoint() || this->updateFacing()) {
		// Determine the rough orientation of the object.
		if (abs(this->facing.z) >= abs(this->facing.x)) {
			if (this->facing.z >= 0.0f) {
				this->trueWestBound  = this->centerX + this->relativeWestBound;
				this->trueEastBound  = this->centerX + this->relativeEastBound;
				this->trueSouthBound = this->centerZ + this->relativeSouthBound;
				this->trueNorthBound = this->centerZ + this->relativeNorthBound;
			}
			else {
				this->trueWestBound  = this->centerX - this->relativeEastBound;
				this->trueEastBound  = this->centerX - this->relativeWestBound;
				this->trueSouthBound = this->centerZ - this->relativeNorthBound;
				this->trueNorthBound = this->centerZ - this->relativeSouthBound;
			}
		}
		else {
			if (this->facing.x >= 0.0f) {
				this->trueWestBound  = this->centerX - this->relativeNorthBound;
				this->trueEastBound  = this->centerX - this->relativeSouthBound;
				this->trueSouthBound = this->centerZ + this->relativeWestBound;
				this->trueNorthBound = this->centerZ + this->relativeEastBound;
			}
			else {
				this->trueWestBound  = this->centerX + this->relativeSouthBound;
				this->trueEastBound  = this->centerX + this->relativeNorthBound;
				this->trueSouthBound = this->centerZ - this->relativeEastBound;
				this->trueNorthBound = this->centerZ - this->relativeWestBound;
			}
		}

		this->updateVisualizer();
	}
}

void GridZone::sendCellEvents() {
	int prevWest  = this->trueWestBound;
	int prevEast  = this->trueEastBound;
	int prevSouth = this->trueSouthBound;
	int prevNorth = this->trueNorthBound;

	this->updateZoneBounds();

	// Don't send messages unless the zone has been added to the grid.
	GameGrid* grid = SINGLETONS->GetGridManager()->GetGrid();
	if (grid->ZoneExistsOnGrid(this->GetObject()->GetId())) {
		int gridWidth = SINGLETONS->GetGridManager()->GetGrid()->GetGridWidth();

		// Get list of cells that the zone occupied before.
		std::list<int> oldCells;
		for (int x = prevWest; x <= prevEast; ++x) {
			for (int z = prevSouth; z <= prevNorth; ++z) {
				oldCells.push_back(x * gridWidth + z);
			}
		}
		oldCells.sort();

		// Get list of cells that the zone occupies now.
		std::list<int> newCells;
		for (int x = this->trueWestBound; x <= this->trueEastBound; ++x) {
			for (int z = this->trueSouthBound; z <= this->trueNorthBound; ++z) {
				newCells.push_back(x * gridWidth + z);
			}
		}
		newCells.sort();

		// Remove the elements that are common to both lists.
		auto oldIter = oldCells.begin();
		auto newIter = newCells.begin();
		while ((oldIter != oldCells.end()) && (newIter != newCells.end())) {
			if (*oldIter < *newIter) {
				oldIter++;
			}
			else if (*oldIter > *newIter) {
				newIter++;
			}
			else {
				oldIter = oldCells.erase(oldIter);
				newIter = newCells.erase(newIter);
			}
		}

		// Send messages based on the cells that were entered or exited.
		for (auto iter = oldCells.begin(); iter != oldCells.end(); ++iter) {
			int x = (*iter) / gridWidth;
			int z = (*iter) % gridWidth;
			MessageChunk enteredCellMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			enteredCellMessage->SetMessageType(MESSAGE_TYPE_GRID_ZONE_EXITED_CELL);
			enteredCellMessage->messageData.iv1.x = x;
			enteredCellMessage->messageData.iv1.z = z;
			ENGINE->GetMessageHub()->SendMulticastMessage(enteredCellMessage, this->GetObject()->GetId());
		}
		for (auto iter = newCells.begin(); iter != newCells.end(); ++iter) {
			int x = (*iter) / gridWidth;
			int z = (*iter) % gridWidth;
			MessageChunk enteredCellMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			enteredCellMessage->SetMessageType(MESSAGE_TYPE_GRID_ZONE_ENTERED_CELL);
			enteredCellMessage->messageData.iv1.x = x;
			enteredCellMessage->messageData.iv1.z = z;
			ENGINE->GetMessageHub()->SendMulticastMessage(enteredCellMessage, this->GetObject()->GetId());
		}
	}
}

void GridZone::onTransformChanged() {
	this->sendCellEvents();
}

void GridZone::handleCameraModeChanged() {
	if (this->visualizerObjectRef == nullptr) {
		return;
	}

	ShadyCamera* shadyCamera = SINGLETONS->GetGridManager()->GetShadyCamera();
	ASSERT(shadyCamera != nullptr, "Got shady camera");
	if (shadyCamera != nullptr) {
		if (shadyCamera->GetCameraMode() == ShadyCamera::CameraMode_Game) {
			this->visualizerObjectRef->SetVisible(this->displayVisualizerInGameMode);

		} else if (shadyCamera->GetCameraMode() == ShadyCamera::CameraMode_Overview) {
			this->visualizerObjectRef->SetVisible(this->displayVisualizerInOverviewMode);

		}

	}
}

void GridZone::Visualize(bool shouldVisualize, std::string imageName, bool displayInGamemode, bool displayInOverviewMode) {
	this->displayVisualizerInGameMode = displayInGamemode;
	this->displayVisualizerInOverviewMode = displayInOverviewMode;

	if (!shouldVisualize) {
		if (this->visualizerObjectRef != nullptr) {
			delete this->visualizerObjectRef;
			this->visualizerObjectRef = nullptr;
		}
	} else {
		if (this->visualizerObjectRef == nullptr) {
			this->visualizerObjectRef = new GameObject(this->gameObjectRef->GetParentSceneGraph());
			this->gameObjectRef->AddChild(this->visualizerObjectRef->GetId());
			// Add a sprite renderer:
			SpriteRenderer* spriteRenderer = this->visualizerObjectRef->GetComponent<SpriteRenderer>();
			if (spriteRenderer == nullptr) {
				spriteRenderer = this->visualizerObjectRef->AddComponent<SpriteRenderer>();
				spriteRenderer->SetHasTransperancy(true);
				std::vector<std::string> pathsToTextures;
				pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + imageName);
				spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);
			}
		}
		this->updateVisualizer();
	}
}

void GridZone::updateVisualizer() {

	if (this->visualizerObjectRef == nullptr) {
		return;
	}

	int x_min, x_max, z_min, z_max;
	this->GetZoneBounds(x_min, x_max, z_min, z_max);
	glm::vec3 offset = ZERO_VEC3;
	offset.x = (this->relativeWestBound + this->relativeEastBound) * 0.5f;
	offset.y = 0.01f;
	offset.z = -(this->relativeSouthBound + this->relativeNorthBound) * 0.5f;
	this->visualizerObjectRef->GetTransform()->SetPosition(offset);
	this->visualizerObjectRef->GetTransform()->SetScale(x_max - x_min + 1, z_max - z_min + 1, z_max - z_min + 1);
	this->visualizerObjectRef->GetTransform()->SetOrientation(-90.0f inRadians, this->visualizerObjectRef->GetTransform()->GetLeft());
}

ObjectIdType GridZone::VisualzerSpriteRendererId(){
	if(this->visualizerObjectRef != nullptr) {
		return this->visualizerObjectRef->GetId();
	}
	return OBJECT_ID_INVALID;
}

void GridZone::init() {
	this->gameObjectRef = (GameObject*)this->GetObject();
	ASSERT(this->gameObjectRef->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");

	this->zoneType = GRID_ZONE_TYPE_UNKNOWN;
	this->centerX            = -1;
	this->centerZ            = -1;
	this->facing             = ZAXIS;
	this->relativeWestBound  = 0;
	this->relativeEastBound  = 0;
	this->relativeSouthBound = 0;
	this->relativeNorthBound = 0;
	this->trueWestBound      = -1;
	this->trueEastBound      = -1;
	this->trueSouthBound     = -1;
	this->trueNorthBound     = -1;

	this->visualizerObjectRef = nullptr;
	this->displayVisualizerInGameMode = true;
	this->displayVisualizerInOverviewMode = true;

	this->updateZoneBounds();

	this->addSubscriptionToMessageType(MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_CAMERA_MODE_CHANGED, this->GetTypeId(), false);
}

void GridZone::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
