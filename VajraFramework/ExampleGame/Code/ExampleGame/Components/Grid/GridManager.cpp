//
//  GridManager.cpp
//  Created by Matt Kaufmann on 12/10/13.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Messages/Declarations.h"

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
	destroy();
}

void GridManager::init() {
	this->cellSize      = 0.0f;
	this->halfCellSize  = ZERO_VEC3;
	this->gridCells     = nullptr;
	this->gridWidth     = 0;
	this->gridHeight    = 0;
	this->roomWidth     = 0;
	this->roomHeight    = 0;
	this->roomOffsetX   = 0;
	this->roomOffsetZ   = 0;
	this->maxElevation  = 0;
	this->gridPlane.origin = ZERO_VEC3;
	this->gridPlane.normal = YAXIS;
#ifdef DEBUG
	// TODO [Remove] Just use this to draw the grid until we get some actual objects into the level
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
#endif
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_CELL_CHANGED, this->GetTypeId(), false);
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
}

void GridManager::HandleMessage(MessageChunk messageChunk) {
	switch (messageChunk->GetMessageType()) {
#ifdef DEBUG
		case MESSAGE_TYPE_FRAME_EVENT:
			DebugDrawGrid();
			DebugTouchTest();
			break;
#endif
		case MESSAGE_TYPE_GRID_CELL_CHANGED:
			gridCellChangedHandler(messageChunk->GetSenderId(), messageChunk->messageData.fv1);
			break;

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
	this->roomWidth = ROOM_WIDTH_OUTDOORS;
	this->roomHeight = ROOM_HEIGHT_OUTDOORS;

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
}

void GridManager::AddGridZone(ObjectIdType zoneId) {
	for (auto iter = this->gridZones.begin(); iter != this->gridZones.end(); ++iter) {
		if (*iter == zoneId) {
			return;
		}
	}
	this->gridZones.push_back(zoneId);
}

int GridManager::GetRoomX(int cellX) {
	return (cellX / this->roomWidth);
}

int GridManager::GetRoomZ(int cellZ) {
	return (cellZ / this->roomHeight);
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

glm::vec3 GridManager::GetRoomCenter(int x, int z) {
	float roomX = this->GetRoomX(x) * this->roomWidth;
	float roomZ = this->GetRoomZ(z) * this->roomHeight;
	roomX += (float)this->roomWidth / 2.0f;
	roomZ += (float)this->roomHeight / -2.0f;
	glm::vec3 center = glm::vec3(roomX, 0.0f, roomZ);
	center -= this->halfCellSize;
	return center;
}

glm::vec3 GridManager::GetRoomCenter(GridCell* cell) {
	return this->GetRoomCenter(cell->x, cell->z);
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


void GridManager::DebugTouchTest() {
	if(ENGINE->GetInput()->GetTouchCount() > 0)
    {
		Touch touch = ENGINE->GetInput()->GetTouch(0);
		GridCell* cell = this->TouchPositionToCell(touch.pos);
		if(cell != nullptr)
			DebugDraw::DrawCube(cell->center, 1.0f);
		glm::vec3 gridPos = this->TouchPositionToGridPosition(touch.pos);
		DebugDraw::DrawCube(gridPos, 1.0f);
    }
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
