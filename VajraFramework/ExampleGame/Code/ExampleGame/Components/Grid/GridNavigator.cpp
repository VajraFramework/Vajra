//
//  GridNavigator.cpp
//  Created by Matt Kaufmann on 12/12/13.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"

#include "Libraries/glm/glm.hpp"

#include "Vajra/Common/Messages/Declarations.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Utilities/MathUtilities.h"

#include <algorithm>
#include <cfloat>
#include <list>
#include <map>

ComponentIdType GridNavigator::componentTypeId = COMPONENT_TYPE_ID_GRID_NAVIGATOR;

GridNavigator::GridNavigator() : Component() {
	this->init();
}

GridNavigator::GridNavigator(Object* object_) : Component(object_) {
	this->init();
}

GridNavigator::~GridNavigator() {
	this->destroy();
}

void GridNavigator::init() {
	this->currentCell = nullptr;
	this->isTraveling = false;
	this->movementSpeed = 1.0f;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void GridNavigator::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void GridNavigator::HandleMessage(MessageChunk messageChunk) {
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_FRAME_EVENT:
			update();
			break;

		default:
			break;
	}
}

void GridNavigator::SetGridPosition(int x, int z) {
	GridCell* cell = SINGLETONS->GetGridManager()->GetCell(x, z);
	if (cell != nullptr) {
		Transform* myTransform = this->GetObject()->GetComponent<Transform>();
		myTransform->SetPosition(cell->center);
		if (this->currentCell != cell) {
			this->changeCell(cell);
		}
	}
}

bool GridNavigator::SetDestination(int x, int z) {
	GridCell* startCell = this->currentCell;
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetCell(x, z);
	std::list<GridCell*> newPath;
	float dist = this->calculatePath(startCell, goalCell, newPath);
	if (dist > 0.0f) {
		this->currentPath = newPath;
		this->isTraveling = true;
	}
	return (dist >= 0.0f);
}

bool GridNavigator::SetDestination(glm::vec3 loc) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetCell(loc);
	if (goalCell != nullptr) {
		SetDestination(goalCell->x, goalCell->z);
	}
	return false;
}

bool GridNavigator::AddDestination(int x, int z) {
	GridCell* startCell = this->currentCell;
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetCell(x, z);
	float dist = this->calculatePath(startCell, goalCell, this->currentPath);
	if (dist > 0.0f) {
		this->isTraveling = true;
	}
	return (dist >= 0.0f);
}

bool GridNavigator::AddDestination(glm::vec3 loc) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetCell(loc);
	if (goalCell != nullptr) {
		AddDestination(goalCell->x, goalCell->z);
	}
	return false;
}

void GridNavigator::update() {
	if (this->isTraveling) {
		followPath();
	}
}

void GridNavigator::followPath() {
	Transform* myTransform = this->GetObject()->GetComponent<Transform>();
	float dt = ENGINE->GetTimer()->GetDeltaFrameTime();
	float distToTravel = this->movementSpeed * dt;
	glm::vec3 tempLocation = myTransform->GetPosition();

	int count = this->currentPath.size();
	while ((distToTravel > 0.0f) && (count > 0)) {
		glm::vec3 targetLocation = this->currentPath.front()->center;
		float distToTarget = glm::distance(tempLocation, targetLocation);

		if (distToTravel >= distToTarget) {
			tempLocation = targetLocation;
			this->currentPath.pop_front();
			--count;
			distToTravel -= distToTarget;
		}
		else {
			float ratio = distToTravel / distToTarget;
			lerp(tempLocation, tempLocation, targetLocation, ratio);
			distToTravel = 0.0f;
		}
	}

	myTransform->SetPosition(tempLocation);
	GridCell* newCell = SINGLETONS->GetGridManager()->GetCell(tempLocation);
	if (newCell != this->currentCell) {
		this->changeCell(newCell);
	}

	if (this->currentPath.size() == 0) {
		this->isTraveling = false;
	}
}

void GridNavigator::changeCell(GridCell* goalCell) {
	// Send a message to the GridManager "asking" to move from one cell to another.
	MessageChunk cellChangeMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	cellChangeMessage->SetMessageType(MESSAGE_TYPE_GRID_CELL_CHANGED);
	cellChangeMessage->messageData.fv1.x = goalCell->x;
	cellChangeMessage->messageData.fv1.y = goalCell->y;
	cellChangeMessage->messageData.fv1.z = goalCell->z;
	ENGINE->GetMessageHub()->SendPointcastMessage(cellChangeMessage, SINGLETONS->GetGridManagerObject()->GetId(), this->GetObject()->GetId());
}

float GridNavigator::calculatePath(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outPath) {
	if ((startCell == nullptr) || (goalCell == nullptr)) {
		return -1.0f;
	}

	int startX = startCell->x;
	int startZ = startCell->z;
	int goalX = goalCell->x;
	int goalZ = goalCell->z;

	// Short-circuit if the start and end points are the same.
	if ((startX == goalX) && (startZ == goalZ)) { return 0.0f; }

	std::list<GridCell*> closedSet;          // Nodes that have been evaluated
	std::list<GridCell*> openSet;            // Nodes that are set to be evaluated
	std::map<GridCell*, GridCell*> cameFrom; // Parent cell along best known path
	std::map<GridCell*, float> gScores;      // Cost from start cell along best known path
	std::map<GridCell*, float> fScores;      // Estimated total cost from start to goal through cell

	gScores[startCell] = 0.0f;
	fScores[startCell] = travelCostEstimate(startCell, goalCell);
	openSet.push_back(startCell);

	while (openSet.size() > 0) {
		// Find the cell in the open set with the lowest fScore
		GridCell* current = nullptr;
		float minFScore = FLT_MAX;
		for (auto iter = openSet.begin(); iter != openSet.end(); ++iter) {
			if (fScores[*iter] < minFScore) {
				minFScore = fScores[*iter];
				current = *iter;
			}
		}

		if (current == goalCell) {
			// Reconstruct and return the path
			GridCell* checked = current;
			while (checked != startCell) {
				outPath.push_front(checked);
				checked = cameFrom[checked];
			}
			return gScores[current];
		}

		openSet.remove(current);
		closedSet.push_back(current);

		std::list<GridCell*> neighbors;
		SINGLETONS->GetGridManager()->GetNeighbors(current, neighbors);
		for (auto iter = neighbors.begin(); iter != neighbors.end(); ++iter) {
			float gScoreTentative = gScores[current] + actualTravelCost(current, *iter);
			float fScoreTentative = gScoreTentative + travelCostEstimate(*iter, goalCell);

			auto closedIter = std::find(closedSet.begin(), closedSet.end(), *iter);

			if ((closedIter != closedSet.end()) && (fScoreTentative >= fScores[*iter])) { continue; }

			auto openIter = std::find(openSet.begin(), openSet.end(), *iter);

			if ((openIter == openSet.end()) || (fScoreTentative < fScores[*iter])) {
				cameFrom[*iter] = current;
				gScores[*iter] = gScoreTentative;
				fScores[*iter] = fScoreTentative;
				if (openIter == openSet.end()) {
					openSet.push_back(*iter);
				}
			}
		}
	}

	return -1.0f; // No path found
}

float GridNavigator::travelCostEstimate(GridCell* startCell, GridCell* goalCell) {
	// Just return the vector distance between the two cells
	return glm::distance(startCell->center, goalCell->center);
}

float GridNavigator::actualTravelCost(GridCell* startCell, GridCell* goalCell) {
	// The travel cost is the vector distance between the two cells
	return glm::distance(startCell->center, goalCell->center);
}