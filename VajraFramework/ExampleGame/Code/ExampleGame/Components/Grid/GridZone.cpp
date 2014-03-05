//
//  GridZone.cpp
//  Created by Matt Kaufmann on 01/07/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridZone.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
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
	case MESSAGE_TYPE_CAMERA_MODE_CHANGED: {
		this->handleCameraModeChanged();
	} break;

	default: {
	} break;
	}
}

void GridZone::GetZoneBounds(int& west, int& east, int& south, int& north) {
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	int centerX, centerZ;
	SINGLETONS->GetGridManager()->GetGrid()->GetCoordinates(centerX, centerZ, trans->GetPositionWorld());

	glm::vec3 forward = QuaternionForwardVector(trans->GetOrientationWorld());

	// Determine the rough orientation of the object.
	if (abs(forward.z) >= abs(forward.x)) {
		if (forward.z <= 0.0f) {
			west  = centerX + this->westBound;
			east  = centerX + this->eastBound;
			south = centerZ + this->southBound;
			north = centerZ + this->northBound;
		}
		else {
			west  = centerX - this->eastBound;
			east  = centerX - this->westBound;
			south = centerZ - this->northBound;
			north = centerZ - this->southBound;
		}
	}
	else {
		if (forward.x <= 0.0f) {
			west  = centerX - this->northBound;
			east  = centerX - this->southBound;
			south = centerZ + this->westBound;
			north = centerZ + this->eastBound;
		}
		else {
			west  = centerX + this->southBound;
			east  = centerX + this->northBound;
			south = centerZ - this->eastBound;
			north = centerZ - this->westBound;
		}
	}
}

void GridZone::SetZoneBounds(int xMin, int zMin, int xMax, int zMax) {
	if (xMin <= xMax) {
		this->westBound = xMin;
		this->eastBound = xMax;
	}
	else {
		this->westBound = xMax;
		this->eastBound = xMin;
	}
	if (zMin <= zMax) {
		this->southBound = zMin;
		this->northBound = zMax;
	}
	else {
		this->southBound = zMax;
		this->northBound = zMin;
	}

	this->updateVisualizer();
}

bool GridZone::IsCellWithinZone(GridCell* cell) {
	if (cell != nullptr) {
		int west, east, south, north;

		this->GetZoneBounds(west, east, south, north);

		return ((cell->x >= west) && (cell->x <= east) && (cell->z >= south) && (cell->z <= north));
	}

	return false;
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
	this->visualizerObjectRef->GetTransform()->SetScale(x_max - x_min + 1, z_max - z_min + 1, z_max - z_min + 1);
	this->visualizerObjectRef->GetTransform()->SetOrientation(-90.0f inRadians, this->visualizerObjectRef->GetTransform()->GetLeft());
	this->visualizerObjectRef->GetTransform()->Translate(0.01f, YAXIS);
}

void GridZone::init() {
	this->gameObjectRef = (GameObject*)this->GetObject();
	ASSERT(this->gameObjectRef->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");

	this->zoneType = GRID_ZONE_TYPE_UNKNOWN;
	this->westBound = -1;
	this->eastBound = -1;
	this->southBound = -1;
	this->northBound = -1;

	this->visualizerObjectRef = nullptr;
	this->displayVisualizerInGameMode = true;
	this->displayVisualizerInOverviewMode = true;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_CAMERA_MODE_CHANGED, this->GetTypeId(), false);
}

void GridZone::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
