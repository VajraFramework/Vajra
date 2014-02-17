//
//  GameGrid.cpp
//  Created by Matt Kaufmann on 02/04/14.
//

#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridConstants.h"
#include "ExampleGame/Components/Grid/GridRoom.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Utilities/MathUtilities.h"

#ifdef DEBUG
#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"
#endif

#include <cfloat>

GameGrid::GameGrid(unsigned int spanX, unsigned int spanZ) {
	this->init(spanX, spanZ);
}

GameGrid::~GameGrid() {
	this->destroy();
}

void GameGrid::init(unsigned int spanX, unsigned int spanZ) {
	// Initialize the grid
	this->gridWidth  = spanX;
	this->gridHeight = spanZ;

	this->gridCells = new GridCell**[this->gridWidth];
	for (unsigned int i = 0; i < this->gridWidth; ++i) {
		this->gridCells[i] = new GridCell*[this->gridHeight];
		for (unsigned int j = 0; j < this->gridHeight; ++j) {
			glm::vec3 center;
			center.x = i * CELL_SIZE;
			center.y = 0;
			center.z = j * -CELL_SIZE;
			glm::vec3 origin = center - HALF_CELL;

			this->gridCells[i][j] = new GridCell(i, 0, j, origin, center);
		}
	}

	// Initialize the bitmaps
	this->passableBits = new unsigned char**[NUM_ELEVATIONS];
	this->visibleBits  = new unsigned char**[NUM_ELEVATIONS];

	// By default, all cells are passable at level 0 and visible at all levels
	for (int i = 0; i < NUM_ELEVATIONS; ++i) {
		this->passableBits[i] = new unsigned char*[this->gridWidth];
		this->visibleBits[i]  = new unsigned char*[this->gridWidth];
		for (unsigned int j = 0; j < this->gridWidth; ++j) {
			int numBytes = (this->gridHeight + 7) / 8;
			this->passableBits[i][j] = new unsigned char[numBytes];
			this->visibleBits[i][j] = new unsigned char[numBytes];
			for (int k = 0; k < numBytes; ++k) {
				if (i == 0) {
					this->passableBits[i][j][k] = 0xFF;
				}
				else {
					this->passableBits[i][j][k] = 0x00;
				}
				this->visibleBits[i][j][k]  = 0xFF;
			}
		}
	}
}

void GameGrid::destroy() {
	// Clear the list of grid zones
	this->gridZones.clear();

	// Free the allocated memory for the grid rooms
	for (auto iter = this->gridRooms.begin(); iter != this->gridRooms.end(); ++iter) {
		delete *iter;
	}
	this->gridRooms.clear();

	// Free the allocated memory for the grid itself
	if (this->gridCells != nullptr) {
		for (unsigned int i = 0; i < this->gridWidth; ++i) {
			for (unsigned int j = 0; j < this->gridHeight; ++j) {
				delete this->gridCells[i][j];
			}
			delete this->gridCells[i];
		}
		delete this->gridCells;
		this->gridCells = nullptr;

		// Free the memory used for the bitmasks
		for (unsigned int i = 0; i < NUM_ELEVATIONS; ++i) {
			for (unsigned int j = 0; j < this->gridWidth; ++j) {
				delete this->passableBits[i][j];
				delete this->visibleBits[i][j];
			}
			delete this->passableBits[i];
			delete this->visibleBits[i];
		}
		delete this->passableBits;
		this->passableBits = nullptr;
		delete this->visibleBits;
		this->visibleBits = nullptr;
	}
	this->gridWidth = 0;
	this->gridHeight = 0;
}

int GameGrid::GetElevationFromWorldY(float worldY) {
	return (int)floor((worldY / ELEVATION_UNIT) + 0.5f);
}

float GameGrid::ConvertElevationToWorldY(unsigned int elevation) {
	return elevation * ELEVATION_UNIT;
}

void GameGrid::GetCoordinates(int& outX, int& outZ, glm::vec3 worldPosition) {
	outX = (int)(( worldPosition.x / CELL_SIZE) + 0.5f);
	outZ = (int)((-worldPosition.z / CELL_SIZE) + 0.5f);
}

GridCell* GameGrid::GetCell(int x, int z) {
	if (isWithinGrid(x * CELL_SIZE, z * CELL_SIZE)) { return this->gridCells[x][z]; }
	return nullptr;
}

GridCell* GameGrid::GetCell(glm::vec3 loc) {
	int gX, gZ;
	this->GetCoordinates(gX, gZ, loc);
	return GetCell(gX, gZ);
}

int GameGrid::GetCellDistanceBetweenCells(int startCellX, int startCellZ, int goalCellX, int goalCellZ) {
	GridCell* startCell = GetCell(startCellX, startCellZ);
	GridCell* goalCell = GetCell(goalCellX, goalCellZ);
	return GetCellDistanceBetweenCells(startCell, goalCell);
}

int GameGrid::GetCellDistanceBetweenCells(GridCell* startCell, GridCell* goalCell) {
	if ((startCell != nullptr) && (goalCell != nullptr)) {
		int xDiff = abs(startCell->x - goalCell->x);
		int zDiff = abs(startCell->z - goalCell->z);
		return xDiff + zDiff;
	}
	// If either cell is null, the distance is undefined
	return INT_MAX;
}

float GameGrid::GetGroundDistanceBetweenCells(int startCellX, int startCellZ, int goalCellX, int goalCellZ) {
	GridCell* startCell = GetCell(startCellX, startCellZ);
	GridCell* goalCell = GetCell(goalCellX, goalCellZ);
	return GetGroundDistanceBetweenCells(startCell, goalCell);
}

float GameGrid::GetGroundDistanceBetweenCells(GridCell* startCell, GridCell* goalCell) {
	if ((startCell != nullptr) && (goalCell != nullptr)) {
		glm::vec3 startCenter = startCell->center;
		glm::vec3 goalCenter = goalCell->center;
		startCenter.y = 0.0f;
		goalCenter.y = 0.0f;
		return glm::distance(startCenter, goalCenter);
	}
	// If either cell is null, the distance is undefined
	return FLT_MAX;
}

float GameGrid::GetTrueDistanceBetweenCells(int startCellX, int startCellZ, int goalCellX, int goalCellZ) {
	GridCell* startCell = GetCell(startCellX, startCellZ);
	GridCell* goalCell = GetCell(goalCellX, goalCellZ);
	return GetTrueDistanceBetweenCells(startCell, goalCell);
}

float GameGrid::GetTrueDistanceBetweenCells(GridCell* startCell, GridCell* goalCell) {
	if ((startCell != nullptr) && (goalCell != nullptr)) {
		return glm::distance(startCell->center, goalCell->center);
	}
	// If either cell is null, the distance is undefined
	return FLT_MAX;
}

void GameGrid::GetNeighborCells(std::list<GridCell*>& outNbrs, GridCell* cell, float range/*= 1.0f*/) {
	glm::vec3 startCenter = cell->center;

	for (int i = 1; i <= (int)range; ++i) {
		for (int j = 0; j <= (int)(range - i); ++j) {
			glm::vec3 targetCenter = startCenter;
			targetCenter.x += i;
			targetCenter.z += j;
			if (glm::distance(startCenter, targetCenter) <= range) {
				if (this->isWithinGrid(cell->x + i, cell->z + j)) {
					outNbrs.push_back(this->gridCells[cell->x + i][cell->z + j]);
				}
				if (this->isWithinGrid(cell->x - j, cell->z + i)) {
					outNbrs.push_back(this->gridCells[cell->x - j][cell->z + i]);
				}
				if (this->isWithinGrid(cell->x - i, cell->z - j)) {
					outNbrs.push_back(this->gridCells[cell->x - i][cell->z - j]);
				}
				if (this->isWithinGrid(cell->x + j, cell->z - i)) {
					outNbrs.push_back(this->gridCells[cell->x + j][cell->z - i]);
				}
			}
		}
	}
}

void GameGrid::AddGridRoom(int west, int south, int width, int height) {
	GridRoom* room = new GridRoom(west, south, width, height);
	this->gridRooms.push_back(room);
}

GridRoom* GameGrid::GetRoom(int x, int z) {
	GridCell* cell = GetCell(x, z);
	return GetRoom(cell);
}

GridRoom* GameGrid::GetRoom(glm::vec3 loc) {
	GridCell* cell = GetCell(loc);
	return GetRoom(cell);
}

GridRoom* GameGrid::GetRoom(GridCell* cell) {
	if (cell != nullptr) {
		for (unsigned int i = 0; i < this->gridRooms.size(); ++i) {
			if (this->gridRooms[i]->IsWithinRoom(cell->x, cell->z)) {
				return this->gridRooms[i];
			}
		}
	}
	return nullptr;
}

glm::vec3 GameGrid::GetRoomCenter(int x, int z) {
	GridCell* cell = GetCell(x, z);
	return GetRoomCenter(cell);
}

glm::vec3 GameGrid::GetRoomCenter(GridCell* cell) {
	glm::vec3 center;
	GridRoom* room = GetRoom(cell);
	if (room != nullptr) {
		center.x = (room->westBound + room->eastBound) * CELL_SIZE * 0.5f;
		center.y = 0.0f;
		center.z = (room->southBound + room->northBound) * CELL_SIZE * -0.5f;
		return center;
	}
	return ZERO_VEC3;
}

void GameGrid::AddGridZone(ObjectIdType zoneId) {
	for (auto iter = this->gridZones.begin(); iter != this->gridZones.end(); ++iter) {
		if (*iter == zoneId) {
			// This zone is already in our list
			return;
		}
	}

	// Add the id to the list
	this->gridZones.push_back(zoneId);
}

GridZone* GameGrid::GetZone(int x, int z) {
	GridCell* cell = GetCell(x, z);
	return GetZone(cell);
}

GridZone* GameGrid::GetZone(glm::vec3 loc) {
	GridCell* cell = GetCell(loc);
	return GetZone(cell);
}

GridZone* GameGrid::GetZone(GridCell* cell) {
	if (cell != nullptr) {
		// If the cell is in multiple zones, only the first one is returned.
		for (auto iter = this->gridZones.begin(); iter != this->gridZones.end(); ++iter) {
			GameObject* zoneObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(*iter);
			GridZone* zone = zoneObj->GetComponent<GridZone>();

			if (zone->IsCellWithinZone(cell)) {
				return zone;
			}
		}
	}
	return nullptr;
}

void GameGrid::TouchedCells(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outTouched) {
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
		incrX = (float)xDirection / spanX;
		fracX = incrX * 0.5f;
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
		incrZ = (float)zDirection / spanZ;
		fracZ = incrZ * 0.5f;
	}

	int xIndex = startCell->x;
	int zIndex = startCell->z;

	// Add the starting cell
	outTouched.push_back(startCell);
	while ((fracX < 1.0f) || (fracZ < 1.0f)) {
		float diff = fracZ - fracX;

		// Find the next cell
		if (diff > ROUNDING_ERROR) {
			outTouched.push_back(this->gridCells[xIndex + xDirection][zIndex]);
			xIndex += xDirection;
			fracX += incrX;
		}
		else if (diff < -ROUNDING_ERROR) {
			outTouched.push_back(this->gridCells[xIndex][zIndex + zDirection]);
			zIndex += zDirection;
			fracZ += incrZ;
		}
		else {
			outTouched.push_back(this->gridCells[xIndex + xDirection][zIndex]);
			outTouched.push_back(this->gridCells[xIndex][zIndex + zDirection]);
			outTouched.push_back(this->gridCells[xIndex + xDirection][zIndex + zDirection]);
			xIndex += xDirection;
			zIndex += zDirection;
			fracX  += incrX;
			fracZ  += incrZ;
		}
	}
}

bool GameGrid::Passable(GridCell* startCell, GridCell* goalCell) {
	if (this->IsCellPassableAtElevation(goalCell->x, goalCell->z, startCell->y)) { return true; }
	return false;
}

bool GameGrid::HasLineOfSight(GridCell* sourceCell, GridCell* targetCell) {
	if (sourceCell != nullptr) {
		return this->HasLineOfSight(sourceCell, targetCell, sourceCell->y);
	}
	return false;
}

bool GameGrid::HasLineOfSight(int sourceCellX, int sourceCellZ, int targetCellX, int targetCellZ) {
	GridCell* sourceCell = this->GetCell(sourceCellX, sourceCellZ);
	GridCell* targetCell = this->GetCell(targetCellX, targetCellZ);
	return this->HasLineOfSight(sourceCell, targetCell);
}

bool GameGrid::HasLineOfSight(GridCell* sourceCell, GridCell* targetCell, unsigned int elevation) {
	// Make sure both cells exist
	if ((sourceCell != nullptr) && (targetCell != nullptr)) {
		// Check the cells along the path
		std::list<GridCell*> touchedCells;
		this->TouchedCells(sourceCell, targetCell, touchedCells);

		for (auto iter = touchedCells.begin(); iter != touchedCells.end(); ++iter) {
			if (!this->IsCellVisibleAtElevation(targetCell->x, targetCell->z, elevation)) {
				return false;
			}
		}

		return true;
	}
	return false;
}

bool GameGrid::HasLineOfSight(int sourceCellX, int sourceCellZ, int targetCellX, int targetCellZ, unsigned int elevation) {
	GridCell* sourceCell = this->GetCell(sourceCellX, sourceCellZ);
	GridCell* targetCell = this->GetCell(targetCellX, targetCellZ);
	return this->HasLineOfSight(sourceCell, targetCell, elevation);
}

int GameGrid::GetCellGroundLevel(int gridX, int gridZ) {
	GridCell* cell = this->GetCell(gridX, gridZ);
	if (cell != nullptr) {
		return cell->y;
	}
	return -1;
}

bool GameGrid::IsCellPassableAtElevation(int gridX, int gridZ, unsigned int elevation) {
	if (elevation < NUM_ELEVATIONS) {
		if (this->isWithinGrid(gridX, gridZ)) {
			return IS_BIT_SET_IN_BYTE_ARRAY(gridZ, this->passableBits[elevation][gridX]);
		}
	}
	return false;
}

bool GameGrid::IsCellVisibleAtElevation(int gridX, int gridZ, unsigned int elevation) {
	if (elevation < NUM_ELEVATIONS) {
		if (this->isWithinGrid(gridX, gridZ)) {
			return IS_BIT_SET_IN_BYTE_ARRAY(gridZ, this->visibleBits[elevation][gridX]);
		}
	}
	return false;
}

void GameGrid::ChangeCellGroundLevel(int gridX, int gridZ, int elevationDiff) {
	if (elevationDiff != 0) {
		GridCell* cell = this->GetCell(gridX, gridZ);

		ASSERT(cell != nullptr, "Cell exists at position (%d, %d)", gridX, gridZ);
		if (cell != nullptr) {
			this->SetCellGroundLevel(gridX, gridZ, cell->y + elevationDiff);
		}
	}
}

void GameGrid::SetCellGroundLevel(int gridX, int gridZ, unsigned int elevation) {
	ASSERT(elevation < NUM_ELEVATIONS, "Elevation level %u is within grid bounds", elevation);
	if (elevation < NUM_ELEVATIONS) {
		GridCell* cell = this->GetCell(gridX, gridZ);

		ASSERT(cell != nullptr, "Cell exists at position (%d, %d)", gridX, gridZ);
		if (cell != nullptr) {
			int diff = elevation - cell->y;
			if (diff > 0) {
				for (int i = NUM_ELEVATIONS - 1; i >= 0; --i) {
					int copyFrom = i - diff;
					bool isPassable, isVisible;
					if (copyFrom >= 0) {
						isPassable = this->IsCellPassableAtElevation(gridX, gridZ, copyFrom);
						isVisible  = this->IsCellVisibleAtElevation(gridX, gridZ, copyFrom);
					}
					else {
						isPassable = false;
						isVisible = false;
					}
					this->SetCellPassableAtElevation(gridX, gridZ, i, isPassable);
					this->SetCellVisibleAtElevation(gridX, gridZ, i, isVisible);
				}
			}
			else if (diff < 0) {
				for (int i = 0; i < NUM_ELEVATIONS; ++i) {
					int copyFrom = i - diff;
					bool isPassable, isVisible;
					if (copyFrom < NUM_ELEVATIONS) {
						isPassable = this->IsCellPassableAtElevation(gridX, gridZ, copyFrom);
						isVisible  = this->IsCellVisibleAtElevation(gridX, gridZ, copyFrom);
					}
					else {
						isPassable = false;
						isVisible = true;
					}
					this->SetCellPassableAtElevation(gridX, gridZ, i, isPassable);
					this->SetCellVisibleAtElevation(gridX, gridZ, i, isVisible);
				}
			}
			cell->y = elevation;
			cell->origin.y = this->ConvertElevationToWorldY(elevation);
			cell->center.y = this->ConvertElevationToWorldY(elevation);
		}
	}
}

void GameGrid::SetCellPassableAtElevation(int gridX, int gridZ, unsigned int elevation, bool isPassable) {
	ASSERT(elevation < NUM_ELEVATIONS, "Elevation level %u is within grid bounds", elevation);
	if (elevation < NUM_ELEVATIONS) {
		ASSERT(this->isWithinGrid(gridX, gridZ), "Cell exists at position (%d, %d)", gridX, gridZ);
		if (this->isWithinGrid(gridX, gridZ)) {
			if (isPassable) {
				SET_BIT_IN_BYTE_ARRAY(gridZ, this->passableBits[elevation][gridX]);
			}
			else {
				UNSET_BIT_IN_BYTE_ARRAY(gridZ, this->passableBits[elevation][gridX]);
			}
		}
	}
}

void GameGrid::SetCellVisibleAtElevation(int gridX, int gridZ, unsigned int elevation, bool isVisible) {
	ASSERT(elevation < NUM_ELEVATIONS, "Elevation level %u is within grid bounds", elevation);
	if (elevation < NUM_ELEVATIONS) {
		ASSERT(this->isWithinGrid(gridX, gridZ), "Cell exists at position (%d, %d)", gridX, gridZ);
		if (this->isWithinGrid(gridX, gridZ)) {
			if (isVisible) {
				SET_BIT_IN_BYTE_ARRAY(gridZ, this->visibleBits[elevation][gridX]);
			}
			else {
				UNSET_BIT_IN_BYTE_ARRAY(gridZ, this->visibleBits[elevation][gridX]);
			}
		}
	}
}

bool GameGrid::isWithinGrid(int x, int z) {
	return (x >= 0) && (x < (int)this->gridWidth) && (z >= 0) && (z < (int)this->gridHeight);
}

bool GameGrid::isWithinGrid(glm::vec3 loc) {
	int gX, gZ;
	this->GetCoordinates(gX, gZ, loc);
	return isWithinGrid(gX, gZ);
}

#ifdef DEBUG_GRID
void GameGrid::debugDrawGrid() {
	glm::vec3 start, end;

	start.x = -CELL_SIZE / 2;
	start.y = 0.0f;
	start.z = CELL_SIZE / 2;
	end.x = -CELL_SIZE / 2;
	end.y = 0.0f;
	end.z = (0.5f - this->gridHeight) * CELL_SIZE;
	unsigned int i = 0;
	do {
		DebugDraw::DrawLine(start, end);
		++i;
		start.x += CELL_SIZE;
		end.x += CELL_SIZE;
	} while (i <= this->gridWidth);

	start.x = -CELL_SIZE / 2;
	start.y = 0.0f;
	start.z = CELL_SIZE / 2;
	end.x = (this->gridWidth - 0.5f) * CELL_SIZE;
	end.y = 0.0f;
	end.z = CELL_SIZE / 2;
	unsigned int j = 0;
	do {
		DebugDraw::DrawLine(start, end);
		++j;
		start.z -= CELL_SIZE;
		end.z -= CELL_SIZE;
	} while (j <= this->gridHeight);
}
#endif
