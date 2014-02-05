//
//  GameGrid.cpp
//  Created by Matt Kaufmann on 02/04/14.
//

#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridConstants.h"
#include "ExampleGame/Components/Grid/GridRoom.h"

GameGrid::GameGrid() {
	this->init();
}

GameGrid::~GameGrid() {
	this->destroy();
}

GridCell* GameGrid::GetCell(int x, int z) {
	if (isWithinGrid(x * CELL_SIZE, z * CELL_SIZE)) { return &(this->gridCells[x][z]); }
	return nullptr;
}

GridCell* GameGrid::GetCell(glm::vec3 loc) {
	int gX = (int)((loc.x / CELL_SIZE) + 0.5f);
	int gZ = (int)((-loc.z / CELL_SIZE) + 0.5f);
	return GetCell(gX, gZ);
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

void GameGrid::GetNeighbors(std::list<GridCell*>& /*outNbrs*/, GridCell* /*cell*/, int /*range= 1*/) {

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
		outTouched.push_back(&(this->gridCells[xIndex + xDirection][zIndex]));
		outTouched.push_back(&(this->gridCells[xIndex][zIndex + zDirection]));

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
			outTouched.push_back(&(this->gridCells[xIndex + xDirection][zIndex + zDirection]));
			xIndex += xDirection;
			zIndex += zDirection;
			fracX  += incrX;
			fracZ  += incrZ;
		}
	}
}

bool GameGrid::HasLineOfSight(GridCell* sourceCell, GridCell* targetCell) {
	// Make sure both cells exist
	if ((sourceCell != nullptr) && (targetCell != nullptr)) {
		// Check the cells along the path
		std::list<GridCell*> touchedCells;
		this->TouchedCells(sourceCell, targetCell, touchedCells);

		for (auto iter = touchedCells.begin(); iter != touchedCells.end(); ++iter) {
			if ((*iter)->y > sourceCell->y) {
				return false;
			}
			if (!(*iter)->isPassable) {
				return false;
			}
		}

		return true;
	}
	return false;
}

bool GameGrid::HasLineOfSight(int sourceCellX, int sourceCellZ, int targetCellX, int targetCellZ) {
	GridCell* sourceCell = this->GetCell(sourceCellX, sourceCellZ);
	GridCell* targetCell = this->GetCell(targetCellX, targetCellZ);
	return this->HasLineOfSight(sourceCell, targetCell);
}

void GameGrid::init() {

}

void GameGrid::destroy() {

}

bool GameGrid::isWithinGrid(int x, int z) {
	return (x >= 0) && (x < (int)this->gridWidth) && (z >= 0) && (z < (int)this->gridHeight);
}

bool GameGrid::isWithinGrid(glm::vec3 loc) {
	int gX = (int)((loc.x / CELL_SIZE) + 0.5f);
	int gZ = (int)((-loc.z / CELL_SIZE) + 0.5f);
	return isWithinGrid(gX, gZ);
}
