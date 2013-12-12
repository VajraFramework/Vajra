//
//  GridManager.cpp
//  Created by Matt Kaufmann on 12/10/13.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"
#include "Vajra/Utilities/MathUtilities.h"

#define INDOOR_SCREEN_WIDTH 15
#define INDOOR_SCREEN_HEIGHT 10
#define OUTDOOR_SCREEN_WIDTH 18
#define OUTDOOR_SCREEN_HEIGHT 12

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
	this->grid          = nullptr;
	this->gridWidth     = 0;
	this->gridHeight    = 0;
	this->screenWidth   = 0;
	this->screenHeight  = 0;
	this->screenOffsetX = 0;
	this->screenOffsetZ = 0;
	this->maxElevation  = 0;
	this->gridOrigin    = ZERO_VEC3;

	// TODO [Remove] Just use this to draw the grid until we get some actual objects into the level
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void GridManager::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());

	if (this->grid != nullptr) {
		for (unsigned int i = 0; i < this->gridWidth; ++i) {
			for (unsigned int j = 0; j < this->gridHeight; ++j) {
				delete this->grid[i][j];
			}
			delete [] this->grid[i];
		}
		delete [] this->grid;
		this->grid = nullptr;
	}

	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void GridManager::HandleMessage(Message* message) {
	switch (message->GetMessageType()) {
		case MESSAGE_TYPE_FRAME_EVENT:
			DebugDrawGrid();
			break;

		default:
			break;
	}
}

void GridManager::GenerateTerrainFromFile(std::string terrainFilename) {
	// TODO [Implement] Psych! We're just creating a default terrain right now

	this->cellSize = 1.0f;
	this->halfCellSize.x = 0.5f;
	this->halfCellSize.y = 0.0f;
	this->halfCellSize.z = 0.5f;
	this->gridWidth = OUTDOOR_SCREEN_WIDTH;
	this->gridHeight = OUTDOOR_SCREEN_HEIGHT;
	this->screenWidth = OUTDOOR_SCREEN_WIDTH;
	this->screenHeight = OUTDOOR_SCREEN_HEIGHT;

	this->grid = new GridCell**[OUTDOOR_SCREEN_WIDTH];
	for (unsigned int i = 0; i < OUTDOOR_SCREEN_WIDTH; ++i) {
		this->grid[i] = new GridCell*[OUTDOOR_SCREEN_HEIGHT];
		for (unsigned int j = 0; j < OUTDOOR_SCREEN_HEIGHT; ++j) {
			glm::vec3 center;
			center.x = i * this->cellSize;
			center.y = 0;
			center.z = j * this->cellSize;
			glm::vec3 origin = center - this->halfCellSize;
			this->grid[i][j] = new GridCell(i, 0, j, origin, center, true);
		}
	}

	this->gridOrigin = this->grid[0][0]->center;
}

int GridManager::GetRoomX(int cellX) {
	return (cellX / this->screenWidth);
}

int GridManager::GetRoomZ(int cellZ) {
	return (cellZ / this->screenHeight);
}

GridCell* GridManager::GetCell(int x, int z) {
	if (IsInCell(x * this->cellSize, z * this->cellSize)) { return nullptr; }

	return this->grid[x][z];
}

GridCell* GridManager::GetCell(glm::vec3 loc) {
	int gX = (int)((loc.x / this->cellSize) + 0.5f);
	int gZ = (int)((loc.z / this->cellSize) + 0.5f);
	return GetCell(gX, gZ);
}
/*
GridCell* GridManager::TouchPositionToCell(glm::vec3 touchPos) {

}

glm::vec3 GridManager::TouchPositionToGridPosition(glm::vec3 touchPos) {

}

void GridManager::ToggleOverview() {

}

void GridManager::TouchOnGrid(uTouch uT) {

}

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
bool GridManager::IsInCell(int x, int z) {
	return (x >= 0) && (x < this->gridWidth) && (z >= 0) && (z < this->gridHeight);
}

bool GridManager::IsInCell(glm::vec3 loc) {
	int gX = (int)((loc.x / this->cellSize) + 0.5f);
	int gZ = (int)((loc.z / this->cellSize) + 0.5f);
	return IsInCell(gX, gZ);
}

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
