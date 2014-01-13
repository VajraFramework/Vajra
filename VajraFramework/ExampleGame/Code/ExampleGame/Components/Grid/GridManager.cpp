//
//  GridManager.cpp
//  Created by Matt Kaufmann on 12/10/13.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Messages/Declarations.h"

#include "Vajra/Common/Messages/Declarations.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Utilities/MathUtilities.h"

#define ROOM_WIDTH_INDOORS 15
#define ROOM_HEIGHT_INDOORS 10
#define ROOM_WIDTH_OUTDOORS 18
#define ROOM_HEIGHT_OUTDOORS 12

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
	this->cellSize      = 0.0f;
	this->halfCellSize  = ZERO_VEC3;
	this->gridCells     = nullptr;
	this->gridWidth     = 0;
	this->gridHeight    = 0;
	this->maxElevation  = 0;
	this->gridPlane.origin = ZERO_VEC3;
	this->gridPlane.normal = YAXIS;
	this->selectedUnitId   = OBJECT_ID_INVALID;
#ifdef DEBUG
	// TODO [Remove] Just use this to draw the grid until we get some actual objects into the level
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
#endif
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_CELL_CHANGED, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_UNIT_KILLED, this->GetTypeId(), false);
}

void GridManager::destroy() {
#ifdef DEBUG
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
#endif
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());

	if (this->gridCells != nullptr) {
		for (unsigned int i = 0; i < this->gridWidth; ++i) {
			for (unsigned int j = 0; j < this->gridHeight; ++j) {
				if (this->gridCells[i][j] != nullptr) {
					delete this->gridCells[i][j];
				}
			}
			delete [] this->gridCells[i];
		}
		delete [] this->gridCells;
		this->gridCells = nullptr;
	}

	for (unsigned int i = 0; i < this->gridRooms.size(); ++i) {
		delete this->gridRooms[i];
	}
	this->gridRooms.clear();
}

void GridManager::HandleMessage(MessageChunk messageChunk) {
	switch (messageChunk->GetMessageType()) {
#ifdef DEBUG
		case MESSAGE_TYPE_FRAME_EVENT:
			DebugDrawGrid();
			break;
#endif
		case MESSAGE_TYPE_GRID_CELL_CHANGED:
			gridCellChangedHandler(messageChunk->GetSenderId(), messageChunk->messageData.fv1);
			break;
		case MESSAGE_TYPE_UNIT_KILLED:
			this->removeNavigatorFromGrid(messageChunk->GetSenderId(), messageChunk->messageData.fv1);
		default:
			break;
	}
}

void GridManager::GenerateTerrainFromFile(std::string /* terrainFilename */) {
	// TODO [Implement] Psych! We're just creating a default terrain right now

	this->cellSize = 1.0f;
	this->halfCellSize.x = 0.5f;
	this->halfCellSize.y = 0.0f;
	this->halfCellSize.z = -0.5f;
	this->gridWidth = ROOM_WIDTH_OUTDOORS * 2.0f;
	this->gridHeight = ROOM_HEIGHT_OUTDOORS * 2.0f;

	this->gridCells = new GridCell**[this->gridWidth];
	for (unsigned int i = 0; i < this->gridWidth; ++i) {
		this->gridCells[i] = new GridCell*[this->gridHeight];
		for (unsigned int j = 0; j < this->gridHeight; ++j) {
			glm::vec3 center;
			center.x = i * this->cellSize;
			center.y = 0;
			center.z = j * -this->cellSize;
			glm::vec3 origin = center - this->halfCellSize;
			this->gridCells[i][j] = new GridCell(i, 0, j, origin, center, true);
		}
	}

	this->gridPlane.origin = this->gridCells[0][0]->center;

	// Set some random cells as blocked to test pathing
	this->gridCells[4][0]->isPassable = false;
	this->gridCells[4][1]->isPassable = false;
	this->gridCells[4][2]->isPassable = false;
	this->gridCells[4][3]->isPassable = false;
	this->gridCells[4][4]->isPassable = false;

	// Spawn sample rooms
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			GridRoom* room = new GridRoom(ROOM_WIDTH_OUTDOORS * i, ROOM_HEIGHT_OUTDOORS * j, ROOM_WIDTH_OUTDOORS, ROOM_HEIGHT_OUTDOORS);
			gridRooms.push_back(room);
		}
	}

	this->gridPlane.origin = this->gridCells[0][0]->center;
}

void GridManager::AddGridZone(ObjectIdType zoneId) {
	for (auto iter = this->gridZones.begin(); iter != this->gridZones.end(); ++iter) {
		if (*iter == zoneId) {
			return;
		}
	}
	this->gridZones.push_back(zoneId);
}

GridCell* GridManager::GetCell(int x, int z) {
	if (IsWithinGrid(x * this->cellSize, z * this->cellSize)) { return this->gridCells[x][z]; }
	return nullptr;
}

GridCell* GridManager::GetCell(glm::vec3 loc) {
	int gX = (int)((loc.x / this->cellSize) + 0.5f);
	int gZ = (int)((-loc.z / this->cellSize) + 0.5f);
	return GetCell(gX, gZ);
}

GridRoom* GridManager::GetRoom(int x, int z) {
	GridCell* cell = GetCell(x, z);
	return GetRoom(cell);
}

GridRoom* GridManager::GetRoom(glm::vec3 loc) {
	int gX = (int)((loc.x / this->cellSize) + 0.5f);
	int gZ = (int)((-loc.z / this->cellSize) + 0.5f);
	return GetRoom(gX, gZ);
}

GridRoom* GridManager::GetRoom(GridCell* cell) {
	if (cell != nullptr) {
		for (unsigned int i = 0; i < this->gridRooms.size(); ++i) {
			if (this->gridRooms[i]->IsWithinRoom(cell->x, cell->z)) {
				return this->gridRooms[i];
			}
		}
	}
	return nullptr;
}

glm::vec3 GridManager::GetRoomCenter(int x, int z) {
	GridCell* cell = GetCell(x, z);
	return GetRoomCenter(cell);
}

glm::vec3 GridManager::GetRoomCenter(GridCell* cell) {
	glm::vec3 center;
	GridRoom* room = GetRoom(cell);
	if (room != nullptr) {
		center.x = (room->westBound + room->eastBound) * this->cellSize * 0.5f;
		center.y = 0.0f;
		center.z = (room->southBound + room->northBound) * this->cellSize * -0.5f;
		return center;
	}
	return ZERO_VEC3;
}

void GridManager::OnTouch(int touchIndex) {
#ifdef DEBUG
	DebugTouchTest(touchIndex);
#endif
	Touch touch = ENGINE->GetInput()->GetTouch(touchIndex);
	GridCell* cell = this->TouchPositionToCell(touch.pos);
	if (cell != nullptr) {
		if (touch.phase == TouchPhase::Began) {
			if (cell->unitId != OBJECT_ID_INVALID) {
				selectUnitInCell(cell);
			}
			else if (this->selectedUnitId != OBJECT_ID_INVALID) {
				GameObject* obj = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->selectedUnitId);
				GridNavigator* gNav = obj->GetComponent<GridNavigator>();
				gNav->SetDestination(cell->x, cell->z);
			}
		}
	}
}

GridCell* GridManager::TouchPositionToCell(glm::vec2 touchPos) {
	glm::vec3 gridPosition = this->TouchPositionToGridPosition(touchPos);
	return this->GetCell(gridPosition);;
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

bool GridManager::Passable(GridCell* /* startCell */, GridCell* goalCell) {
	if (goalCell->isPassable) { return true; }
	return false;
}

void GridManager::TouchedCells(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outTouched) {
	int spanX = goalCell->x - startCell->x;
	int spanZ = goalCell->z - startCell->z;

	int xDirection;
	float fracX, incrX;
	if (spanX == 0) {
		xDirection = 0;
		fracX = 100.0f;
		incrX = 100.0f;
	}
	else {
		if (spanX > 0)  { xDirection =  1; }
		else            { xDirection = -1; }
		fracX = 0.0f;
		incrX = (float)xDirection / spanX;
	}

	int zDirection;
	float fracZ, incrZ;
	if (spanZ == 0) {
		zDirection = 0;
		fracZ = 100.0f;
		incrZ = 100.0f;
	}
	else {
		if (spanZ > 0)  { zDirection =  1; }
		else            { zDirection = -1; }
		fracZ = 0.0f;
		incrZ = (float)zDirection / spanZ;
	}

	int xIndex = startCell->x;
	int zIndex = startCell->z;

	while ((fracX < 1.0f) || (fracZ < 1.0f)) {
		outTouched.push_back(this->gridCells[xIndex + xDirection][zIndex]);
		outTouched.push_back(this->gridCells[xIndex][zIndex + zDirection]);

		float diff = (fracZ + incrZ) - (fracX + incrX);
		const float ERROR_MARGIN = 0.0001f;

		// Find the next cell
		if (diff > ERROR_MARGIN) {
			xIndex += xDirection;
			fracX += incrX;
		}
		else if (diff < -ERROR_MARGIN) {
			zIndex += zDirection;
			fracZ += incrZ;
		}
		else {
			outTouched.push_back(this->gridCells[xIndex + xDirection][zIndex + zDirection]);
			xIndex += xDirection;
			zIndex += zDirection;
			fracX  += incrX;
			fracZ  += incrZ;
		}
	}
}
/*
void GridManager::ToggleOverview() {

}

void GridManager::TouchOnGrid(uTouch uT) {

}
*/
void GridManager::GetNeighbors(GridCell* cel, std::list<GridCell*>& outNbrs) {
	if (IsWithinGrid(cel->x - 1, cel->z)) {
		outNbrs.push_back(this->gridCells[cel->x - 1][cel->z]);
	}
	if (IsWithinGrid(cel->x + 1, cel->z)) {
		outNbrs.push_back(this->gridCells[cel->x + 1][cel->z]);
	}
	if (IsWithinGrid(cel->x, cel->z - 1)) {
		outNbrs.push_back(this->gridCells[cel->x][cel->z - 1]);
	}
	if (IsWithinGrid(cel->x, cel->z + 1)) {
		outNbrs.push_back(this->gridCells[cel->x][cel->z + 1]);
	}
}
/*
std::list<GridCell> GridManager::GetNeighbors(GridCell* cel, bool diagonals, bool sameRoom) {

}

GRID_DIR GridManager::GetVectorDirection(glm::vec3 vec) {

}

std::list<GridCell> GridManager::GetNeighborsInRange(glm::vec3 pos, int range, bool includeObstructed, bool lineOfSight, bool sameElevation, GRID_DIR dir) {

}

bool GridManager::HasLineOfSight(int startX, int startZ, int endX, int endZ) {

}

std::list<GridCell> GridManager::DirectRoute(int startX, int startZ, int endX, int endZ) {

}
*/
bool GridManager::IsWithinGrid(int x, int z) {
	return (x >= 0) && (x < (int)this->gridWidth) && (z >= 0) && (z < (int)this->gridHeight);
}

bool GridManager::IsWithinGrid(glm::vec3 loc) {
	int gX = (int)((loc.x / this->cellSize) + 0.5f);
	int gZ = (int)((-loc.z / this->cellSize) + 0.5f);
	return IsWithinGrid(gX, gZ);
}
#ifdef DEBUG
void GridManager::DebugDrawGrid() {
	glm::vec3 start, end;

	start.x = -this->cellSize / 2;
	start.y = 0.0f;
	start.z = this->cellSize / 2;
	end.x = -this->cellSize / 2;
	end.y = 0.0f;
	end.z = (0.5f - this->gridHeight) * this->cellSize;
	unsigned int i = 0;
	do {
		DebugDraw::DrawLine(start, end);
		++i;
		start.x += this->cellSize;
		end.x += this->cellSize;
	} while (i <= this->gridWidth);

	start.x = -this->cellSize / 2;
	start.y = 0.0f;
	start.z = this->cellSize / 2;
	end.x = (this->gridWidth - 0.5f) * this->cellSize;
	end.y = 0.0f;
	end.z = this->cellSize / 2;
	unsigned int j = 0;
	do {
		DebugDraw::DrawLine(start, end);
		++j;
		start.z -= this->cellSize;
		end.z -= this->cellSize;
	} while (j <= this->gridHeight);
}


void GridManager::DebugTouchTest(int touchIndex) {
	Touch touch = ENGINE->GetInput()->GetTouch(touchIndex);
	GridCell* cell = this->TouchPositionToCell(touch.pos);
	if (cell != nullptr) {
		DebugDraw::DrawCube(cell->center, 1.0f);
	}
	glm::vec3 gridPos = this->TouchPositionToGridPosition(touch.pos);
	DebugDraw::DrawCube(gridPos, 1.0f);
}
#endif

void GridManager::gridCellChangedHandler(ObjectIdType id, glm::vec3 dest) {
	GameObject* obj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	GridCell* destCell = GetCell(dest.x, dest.z);

	GridNavigator* gNav = obj->GetComponent<GridNavigator>();
	ASSERT(gNav != nullptr, "Moving object has GridNavigator component");

	if (destCell->unitId == OBJECT_ID_INVALID) {
		GridCell* startCell = gNav->GetCurrentCell();
		if (startCell != nullptr) {
			startCell->unitId = OBJECT_ID_INVALID;
		}
		destCell->unitId = id;
		gNav->SetCurrentCell(destCell);
		// Determine if the object entered or exited any grid zones.
		this->checkZoneCollisions(id, startCell, destCell);
	}
	else {
		// A collision has occurred. Send a message to both units involved.
		MessageChunk collisionMessageA = ENGINE->GetMessageHub()->GetOneFreeMessage();
		collisionMessageA->SetMessageType(MESSAGE_TYPE_GRID_UNIT_COLLISION);
		collisionMessageA->messageData.i = id;
		ENGINE->GetMessageHub()->SendPointcastMessage(collisionMessageA, destCell->unitId, id);

		MessageChunk collisionMessageB = ENGINE->GetMessageHub()->GetOneFreeMessage();
		collisionMessageB->SetMessageType(MESSAGE_TYPE_GRID_UNIT_COLLISION);
		collisionMessageB->messageData.i = destCell->unitId;
		ENGINE->GetMessageHub()->SendPointcastMessage(collisionMessageB, id, destCell->unitId);
	}
}

void GridManager::removeNavigatorFromGrid(ObjectIdType id, glm::vec3 cellPos) {
	GridCell* cell = this->GetCell(cellPos);
	if(cell->unitId == id) {
		cell->unitId = OBJECT_ID_INVALID;
	}
}
void GridManager::checkZoneCollisions(ObjectIdType id, GridCell* startCell, GridCell* destCell) {
	for (auto iter = this->gridZones.begin(); iter != this->gridZones.end(); ++iter) {
		Object* zoneObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(*iter);
		GridZone* zoneComp = zoneObj->GetComponent<GridZone>();
		MessageType collisionType = zoneComp->CollisionCheck(startCell, destCell);
		if (collisionType != MESSAGE_TYPE_UNSPECIFIED) {
			MessageChunk collisionMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			collisionMessage->SetMessageType(collisionType);
			collisionMessage->messageData.i = id;
			ENGINE->GetMessageHub()->SendPointcastMessage(collisionMessage, *iter, id);
		}
	}
}

void GridManager::selectUnitInCell(int x, int z) {
	this->selectUnitInCell(GetCell(x, z));
}

void GridManager::selectUnitInCell(GridCell* cell) {
	if (cell->unitId != OBJECT_ID_INVALID) {
		GameObject* obj = ENGINE->GetSceneGraph3D()->GetGameObjectById(cell->unitId);
		BaseUnit* bu = obj->GetComponent<BaseUnit>();
		ASSERT(bu != nullptr, "Selected object has BaseUnit component");
		if(bu->GetUnitType() < UnitType::UNIT_TYPE_PLAYER_UNITS_COUNT) {
			this->selectedUnitId = cell->unitId;
		}
	}
}
