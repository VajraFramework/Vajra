//
//  GridManager.cpp
//  Created by Matt Kaufmann on 12/10/13.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridConstants.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/LevelManager/LevelFileTags.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"

#include "Vajra/Common/Messages/Declarations.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Utilities/MathUtilities.h"

#include <sstream>

unsigned int GridManager::componentTypeId = COMPONENT_TYPE_ID_GRID_MANAGER;

GridManager::GridManager() {
	this->init();
}

GridManager::GridManager(Object* object_) : Component(object_) {
	this->init();
}

GridManager::~GridManager() {
	this->destroy();
}

void GridManager::init() {
	this->grid = nullptr;
	this->maxElevation  = 0;
	this->gridPlane.origin = ZERO_VEC3;
	this->gridPlane.normal = YAXIS;
	this->selectedUnitId   = OBJECT_ID_INVALID;
#ifdef DEBUG_GRID
	// TODO [Remove] Just use this to draw the grid until we get some actual objects into the level
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
#endif
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_CELL_CHANGED, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_UNIT_KILLED, this->GetTypeId(), false);
}

void GridManager::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());

	delete this->grid;
	this->grid = nullptr;
}

void GridManager::HandleMessage(MessageChunk messageChunk) {
	switch (messageChunk->GetMessageType()) {
#if DEBUG_GRID
		case MESSAGE_TYPE_FRAME_EVENT:
			this->grid->debugDrawGrid();
			break;
#endif
		case MESSAGE_TYPE_GRID_CELL_CHANGED:
			gridCellChangedHandler(messageChunk->GetSenderId(), messageChunk->messageData.fv1);
			break;
		case MESSAGE_TYPE_UNIT_KILLED:
			this->removeNavigatorFromGrid(messageChunk->GetSenderId(), messageChunk->messageData.fv1);
			break;
		default:
			break;
	}
}

ObjectIdType GridManager::GetPlayerUnitIdOfType(UnitType uType) {
	auto iter = this->playerUnits.find(uType);
	if (iter != this->playerUnits.end()) {
		return iter->second;
	}
	return OBJECT_ID_INVALID;
}

void GridManager::OnTouchUpdate(int touchIndex) {
#ifdef DEBUG_GRID
	debugTouchUpdate(touchIndex);
#endif
	Touch touch = ENGINE->GetInput()->GetTouch(touchIndex);
	GridCell* cell = this->TouchPositionToCell(touch.pos);
	if (cell != nullptr) {
		if(cell->GetFirstOccupantId() != OBJECT_ID_INVALID) {
			if((cell->GetFirstOccupantId() != selectedUnitId) && (touch.phase == TouchPhase::Began)) {
				selectUnitInCell(cell);
			}
		}	
		if (this->selectedUnitId != OBJECT_ID_INVALID) {
			GameObject* obj = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->selectedUnitId);
			PlayerUnit* unit = obj->GetComponent<PlayerUnit>();
			unit->OnTouch(touchIndex, cell);
		}
	}
}

GridCell* GridManager::TouchPositionToCell(glm::vec2 touchPos) {
	glm::vec3 gridPosition = this->TouchPositionToGridPosition(touchPos);
	return this->GetGrid()->GetCell(gridPosition);
}

glm::vec3 GridManager::TouchPositionToGridPosition(glm::vec2 touchPos) {
	glm::vec3 gridPosition = glm::vec3();

	Ray screenRay = ENGINE->GetSceneGraph3D()->GetMainCamera()->ScreenPointToRay(touchPos);
	float dist;
	if(rayPlaneIntersection(screenRay, this->gridPlane, dist))
	{
		gridPosition = screenRay.origin + screenRay.dir * dist;
	}
	return gridPosition;
}
/*
void GridManager::ToggleOverview() {

}

void GridManager::TouchOnGrid(uTouch uT) {

}

GRID_DIR GridManager::GetVectorDirection(glm::vec3 vec) {

}

std::list<GridCell> GridManager::GetNeighborsInRange(glm::vec3 pos, int range, bool includeObstructed, bool lineOfSight, bool sameElevation, GRID_DIR dir) {

}
*/
#ifdef DEBUG_GRID
void GridManager::debugTouchUpdate(int touchIndex) {
	Touch touch = ENGINE->GetInput()->GetTouch(touchIndex);
	GridCell* cell = this->TouchPositionToCell(touch.pos);
	if (cell != nullptr) {
		DebugDraw::DrawCube(cell->center, 1.0f);
	}
	glm::vec3 gridPos = this->TouchPositionToGridPosition(touch.pos);
	DebugDraw::DrawCube(gridPos, 0.1f);
}
#endif

void GridManager::loadGridDataFromXml(XmlNode* gridNode) {
	// Cell Information
	XmlNode* cellDataNode = gridNode->GetFirstChildByNodeName(CELL_DATA_TAG);
	if (cellDataNode != nullptr) {
		this->loadCellDataFromXml(cellDataNode);
	}

	// Room Information
	XmlNode* roomDataNode = gridNode->GetFirstChildByNodeName(ROOM_DATA_TAG);
	if (roomDataNode != nullptr) {
		this->loadRoomDataFromXml(roomDataNode);
	}

	// Eventually this needs to be a list from highest to lowest.
	this->gridPlane.origin = this->grid->GetCell(0, 0)->center;
}

void GridManager::loadCellDataFromXml(XmlNode* cellDataNode) {
	int gridWidth  = cellDataNode->GetAttributeValueI(WIDTH_ATTRIBUTE);
	int gridHeight = cellDataNode->GetAttributeValueI(HEIGHT_ATTRIBUTE);
	this->grid = new GameGrid(gridWidth, gridHeight);

	// Cell Elevations
	XmlNode* elevationNode = cellDataNode->GetFirstChildByNodeName(CELL_ELEVATIONS_TAG);
	if (elevationNode != nullptr) {
		std::stringstream dataStream;
		dataStream.str(elevationNode->GetValue());
		for (int i = (int)this->grid->GetGridHeight() - 1; i >= 0; --i) {
			for (unsigned int j = 0; j < this->grid->GetGridWidth(); ++j) {
				int elevation;
				dataStream >> elevation;

				this->grid->SetCellGroundLevel(j, i, elevation);
			}
		}
	}

	// Passable cells
	XmlNode* passableNode = cellDataNode->GetFirstChildByNodeName(PASSABLE_CELLS_TAG);
	if (passableNode != nullptr) {
		std::stringstream dataStream;
		dataStream.str(passableNode->GetValue());
		for (int i = (int)this->grid->GetGridHeight() - 1; i >= 0; --i) {
			for (unsigned int j = 0; j < this->grid->GetGridWidth(); ++j) {
				int passInt;
				dataStream >> passInt;
				bool canPass = (passInt == 1);

				GridCell* cell = this->grid->GetCell(j, i);
				this->grid->SetCellPassableAtElevation(j, i, cell->y, canPass);
			}
		}
	}

	// Visible cells
	XmlNode* visibleNode = cellDataNode->GetFirstChildByNodeName(VISIBLE_CELLS_TAG);
	if (visibleNode != nullptr) {
		std::stringstream dataStream;
		dataStream.str(visibleNode->GetValue());
		for (int i = (int)this->grid->GetGridHeight() - 1; i >= 0; --i) {
			for (unsigned int j = 0; j < this->grid->GetGridWidth(); ++j) {
				int passInt;
				dataStream >> passInt;
				bool canPass = (passInt == 1);

				GridCell* cell = this->grid->GetCell(j, i);
				this->grid->SetCellVisibleAtElevation(j, i, cell->y, canPass);
			}
		}
	}
}

void GridManager::loadRoomDataFromXml(XmlNode* roomDataNode) {
	XmlNode* roomNode = roomDataNode->GetFirstChildByNodeName(ROOM_TAG);
	while (roomNode != nullptr) {
		int roomX      = roomNode->GetAttributeValueI(X_ATTRIBUTE);
		int roomZ      = roomNode->GetAttributeValueI(Z_ATTRIBUTE);
		int roomWidth  = roomNode->GetAttributeValueI(WIDTH_ATTRIBUTE);
		int roomHeight = roomNode->GetAttributeValueI(HEIGHT_ATTRIBUTE);

		this->grid->AddGridRoom(roomX, roomZ, roomWidth, roomHeight);

		roomNode = roomNode->GetNextSiblingByNodeName(ROOM_TAG);
	}
}

void GridManager::placeStaticObjectOnGrid(ObjectIdType id, int westBound, int southBound, int width, int height) {
	int eastBound = westBound + width - 1;
	int northBound = southBound + height - 1;

	// Set the object's position in the world.
	GameObject* staticObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	staticObj->GetTransform()->SetPosition(westBound + (width - 1) / 2.0f, 0.0f, -(southBound + (height - 1) / 2.0f));

	// Make sure that the object lies entirely within the grid boundaries.
	GridCell* swCornerCell = this->grid->GetCell(westBound, southBound);
	GridCell* neCornerCell = this->grid->GetCell(eastBound, northBound);
	ASSERT((swCornerCell != nullptr) && (neCornerCell != nullptr),
			"Static object covers cell range from (%d, %d) to (%d, %d)",
			westBound, southBound, eastBound, northBound);

	// Add the static object's id to every cell it touches.
	for (int i = westBound; i <= eastBound; ++i) {
		for (int j = southBound; j <= northBound; ++j) {
			this->grid->GetCell(i, j)->staticObjs.push_back(id);
		}
	}
}

void GridManager::placeUnitOnGrid(ObjectIdType id, int cellX, int cellZ) {
	GameObject* obj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	ASSERT(obj != nullptr, "Object with id %d exists", id);

	BaseUnit* unit = obj->GetComponent<BaseUnit>();
	ASSERT(unit != nullptr, "Object with id %d has BaseUnit component", id);
	UnitType uType = unit->GetUnitType();

	if (uType < UNIT_TYPE_PLAYER_UNITS_COUNT) {
		auto iter = this->playerUnits.find(uType);
		ASSERT(iter == this->playerUnits.end(), "Grid does not already have player unit of type %d", uType);
		this->playerUnits[uType] = id;
	}

	GridCell* destCell = this->grid->GetCell(cellX, cellZ);

	GridNavigator* gNav = obj->GetComponent<GridNavigator>();
	ASSERT(gNav != nullptr, "Object with id %d has GridNavigator component", id);
	ASSERT(destCell != nullptr, "Placing object into grid cell (%d, %d)", cellX, cellZ);
	ASSERT(destCell->GetFirstOccupantId() == OBJECT_ID_INVALID, "Grid cell (%d, %d) is unoccupied", cellX, cellZ);

	destCell->SetFirstOccupantId(id);
	gNav->SetCurrentCell(destCell);

	Transform* trans = obj->GetTransform();
	trans->SetPosition(destCell->center);
}

void GridManager::gridCellChangedHandler(ObjectIdType id, glm::vec3 dest) {
	GameObject* obj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	GridCell* destCell = this->grid->GetCell(dest.x, dest.z);

	GridNavigator* gNav = obj->GetComponent<GridNavigator>();
	ASSERT(gNav != nullptr, "Moving object has GridNavigator component");
	GridCell* startCell = gNav->GetCurrentCell();

	if (destCell->GetFirstOccupantId() == OBJECT_ID_INVALID) {
		if (startCell != nullptr) {
			startCell->SetFirstOccupantId(OBJECT_ID_INVALID);
		}
		destCell->SetFirstOccupantId(id);
		gNav->SetCurrentCell(destCell);
		// Determine if the object entered or exited any grid zones.
		this->checkZoneCollisions(id, startCell, destCell);
	}
	else {
		// A collision has occurred. Send a message to both units involved.
		MessageChunk collisionMessageA = ENGINE->GetMessageHub()->GetOneFreeMessage();
		collisionMessageA->SetMessageType(MESSAGE_TYPE_GRID_UNIT_COLLISION);
		collisionMessageA->messageData.iv1.x = id;
		ENGINE->GetMessageHub()->SendPointcastMessage(collisionMessageA, destCell->GetFirstOccupantId(), id);

		MessageChunk collisionMessageB = ENGINE->GetMessageHub()->GetOneFreeMessage();
		collisionMessageB->SetMessageType(MESSAGE_TYPE_GRID_UNIT_COLLISION);
		collisionMessageB->messageData.iv1.x = destCell->GetFirstOccupantId();
		ENGINE->GetMessageHub()->SendPointcastMessage(collisionMessageB, id, destCell->GetFirstOccupantId());
	}

	// Check if the unit changed rooms
	GridRoom* startRoom = this->grid->GetRoom(startCell);
	GridRoom* destRoom = this->grid->GetRoom(destCell);

	if (startRoom != destRoom) {
		if (startRoom != nullptr) {
			MessageChunk roomExitMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			roomExitMessage->SetMessageType(MESSAGE_TYPE_GRID_ROOM_EXITED);
			roomExitMessage->messageData.iv1.x = id;
			roomExitMessage->messageData.fv1 = this->grid->GetRoomCenter(startCell);
			ENGINE->GetMessageHub()->SendMulticastMessage(roomExitMessage, this->GetObject()->GetId());
		}

		if (destRoom != nullptr) {
			MessageChunk roomEnterMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			roomEnterMessage->SetMessageType(MESSAGE_TYPE_GRID_ROOM_ENTERED);
			roomEnterMessage->messageData.iv1.x = id;
			roomEnterMessage->messageData.fv1 = this->grid->GetRoomCenter(destCell);
			ENGINE->GetMessageHub()->SendMulticastMessage(roomEnterMessage, this->GetObject()->GetId());
		}
	}
}

void GridManager::removeNavigatorFromGrid(ObjectIdType id, glm::vec3 cellPos) {
	GridCell* cell = this->grid->GetCell(cellPos);
	if(cell->GetFirstOccupantId() == id) {
		cell->SetFirstOccupantId(OBJECT_ID_INVALID);
	}
}

void GridManager::checkZoneCollisions(ObjectIdType id, GridCell* startCell, GridCell* destCell) {
	// We'll assume that no grid zones overlap
	GridZone* startZone = this->grid->GetZone(startCell);
	GridZone* destZone  = this->grid->GetZone(destCell);

	if (startZone != destZone) {
		// Did the object leave its initial zone?
		if (startZone != nullptr) {
			Object* startZoneObj = startZone->GetObject();
			ObjectIdType startZoneObjId = startZoneObj->GetId();

			MessageChunk collisionMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			collisionMessage->SetMessageType(MESSAGE_TYPE_GRID_ZONE_EXITED);
			collisionMessage->messageData.iv1.x = id;
			ENGINE->GetMessageHub()->SendPointcastMessage(collisionMessage, startZoneObjId, id);
		}

		// Did the object enter a new zone?
		if (destZone != nullptr) {
			Object* destZoneObj = destZone->GetObject();
			ObjectIdType destZoneObjId = destZoneObj->GetId();

			MessageChunk collisionMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			collisionMessage->SetMessageType(MESSAGE_TYPE_GRID_ZONE_ENTERED);
			collisionMessage->messageData.iv1.x = id;
			ENGINE->GetMessageHub()->SendPointcastMessage(collisionMessage, destZoneObjId, id);
		}
	}
}

void GridManager::selectUnitInCell(int x, int z) {
	this->selectUnitInCell(this->grid->GetCell(x, z));
}

void GridManager::selectUnitInCell(GridCell* cell) {
	if (cell->GetFirstOccupantId() != OBJECT_ID_INVALID) {
		GameObject* obj = ENGINE->GetSceneGraph3D()->GetGameObjectById(cell->GetFirstOccupantId());
		BaseUnit* bu = obj->GetComponent<BaseUnit>();
		ASSERT(bu != nullptr, "Selected object has BaseUnit component");
		if(bu->GetUnitType() < UnitType::UNIT_TYPE_PLAYER_UNITS_COUNT) {
			this->deselectUnit();
			this->selectedUnitId = cell->GetFirstOccupantId();
		}
	}	
}

void GridManager::deselectUnit() {
	if (this->selectedUnitId != OBJECT_ID_INVALID) {
		GameObject* obj = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->selectedUnitId);
		BaseUnit* bu = obj->GetComponent<BaseUnit>();
		ASSERT(bu != nullptr, "Selected object has BaseUnit component");
		if(bu->GetUnitType() < UnitType::UNIT_TYPE_PLAYER_UNITS_COUNT) {
			PlayerUnit* pu = obj->GetComponent<PlayerUnit>();
			pu->OnDeselect();
			this->selectedUnitId = OBJECT_ID_INVALID;
		}
	}
}
#if 0
void GridManager::longPressOnGrid() {
	LongPress lp = ENGINE->GetInput()->GetLongPress();
	GridCell* c = this->TouchPositionToCell(lp.pos);
	if(c->GetFirstOccupantId() != OBJECT_ID_INVALID) {
		if(c->GetFirstOccupantId() == selectedUnitId) {
			
		}
		else {
			selectUnitInCell(c);
		}
	}	
}
#endif
