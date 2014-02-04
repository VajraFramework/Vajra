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
#include "Libraries/glm/gtx/vector_angle.hpp"

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
	// TODO [Implement] Ensure type-safety here
	this->gameObjectRef = (GameObject*)this->GetObject();

	this->currentCell = nullptr;
	this->isTraveling = false;
	this->isTurning = false;
	this->movementSpeed = 1.0f;
	this->turningSpeed = 90.0f inRadians;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void GridNavigator::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

GridCell* GridNavigator::GetDestination() {
	if (this->currentPath.size() == 0) {
		return nullptr;
	}
	return this->currentPath.back();
}

void GridNavigator::SetGridPosition(int x, int z) {
	GridCell* cell = SINGLETONS->GetGridManager()->GetCell(x, z);
	this->SetGridPosition(cell);
}

void GridNavigator::SetGridPosition(glm::vec3 loc) {
	GridCell* cell = SINGLETONS->GetGridManager()->GetCell(loc);
	this->SetGridPosition(cell);
}

void GridNavigator::SetGridPosition(GridCell* cell) {
	if (cell != nullptr) {
		Transform* myTransform = this->GetObject()->GetComponent<Transform>();
		myTransform->SetPosition(cell->center);
		if (this->currentCell != cell) {
			this->changeCell(cell);
		}
	}
}

bool GridNavigator::SetDestination(int x, int z) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetCell(x, z);
	return SetDestination(goalCell);
}

bool GridNavigator::SetDestination(glm::vec3 loc) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetCell(loc);
	return SetDestination(goalCell);
}

bool GridNavigator::SetDestination(GridCell* cell) {
	std::list<GridCell*> newPath;
	float dist = this->calculatePath(this->currentCell, cell, newPath);
	if (dist > 0.0f) {
		this->currentPath = newPath;
		this->isTraveling = true;
	}
	return (dist >= 0.0f);
}

bool GridNavigator::AddDestination(int x, int z) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetCell(x, z);
	return AddDestination(goalCell);
}

bool GridNavigator::AddDestination(glm::vec3 loc) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetCell(loc);
	return AddDestination(goalCell);
}

bool GridNavigator::AddDestination(GridCell* cell) {
	if (this->currentPath.size() == 0) {
		return SetDestination(cell);
	}

	float dist = this->calculatePath(this->currentCell, cell, this->currentPath);
	if (dist > 0.0f) {
		this->isTraveling = true;
	}
	return (dist >= 0.0f);
}

void GridNavigator::SetLookTarget(int x, int z) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetCell(x, z);
	if (goalCell != nullptr) {
		SetLookTarget(goalCell->center);
	}
}

void GridNavigator::SetLookTarget(glm::vec3 loc) {
	glm::vec3 target = glm::normalize(loc - this->getTransform()->GetPositionWorld());
	float angle = glm::angle(target, this->getTransform()->GetForward());

	if (angle > ROUNDING_ERROR) {
		this->targetForward = target;
		this->isTurning = true;
	}
}

void GridNavigator::SetLookTarget(glm::quat orient) {
	glm::vec3 target = QuaternionForwardVector(orient);
	float angle = glm::angle(target, this->getTransform()->GetForward());

	if (angle > ROUNDING_ERROR) {
		this->targetForward = target;
		this->isTurning = true;
	}
}

bool GridNavigator::CanReachDestination(int cellX, int cellZ, float maxDistance/*= -1.0f*/) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetCell(cellX, cellZ);
	return CanReachDestination(goalCell, maxDistance);
}

bool GridNavigator::CanReachDestination(glm::vec3 worldPos, float maxDistance/*= -1.0f*/) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetCell(worldPos);
	return CanReachDestination(goalCell, maxDistance);
}

bool GridNavigator::CanReachDestination(GridCell* cell, float maxDistance/*= -1.0f*/) {
	std::list<GridCell*> dummyPath;
	float distance = this->calculatePath(this->currentCell, cell, dummyPath);

	// If maxDist is negative, the path can be any length
	if (maxDistance < 0.0f) {
		return (distance >= 0.0f);
	}
	return ((distance >= 0.0f) && (distance <= maxDistance));
}

void GridNavigator::update() {
	if (this->isTraveling) {
		followPath();
	}
	if (this->isTurning) {
		updateFacing();
	}
}

void GridNavigator::followPath() {
	float dt = ENGINE->GetTimer()->GetDeltaFrameTime();
	float distToTravel = this->movementSpeed * dt;
	Transform* trans = getTransform(); // Store the reference locally to save on function calls.

	glm::vec3 tempLocation = trans->GetPosition();

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

	trans->SetPosition(tempLocation);
	GridCell* newCell = SINGLETONS->GetGridManager()->GetCell(tempLocation);
	if (newCell != this->currentCell) {
		this->changeCell(newCell);
	}

	if (this->currentPath.size() > 0) {
		SetLookTarget(this->currentPath.front()->center);
	}
	else {
		this->isTraveling = false;

		// Send an event message to the unit
		ObjectIdType myId = this->GetObject()->GetId();
		MessageChunk reachedDestinationMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
		reachedDestinationMessage->SetMessageType(MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION);
		reachedDestinationMessage->messageData.fv1 = trans->GetPositionWorld();
		ENGINE->GetMessageHub()->SendPointcastMessage(reachedDestinationMessage, myId, myId);
	}
}

void GridNavigator::updateFacing() {
	float dt = ENGINE->GetTimer()->GetDeltaFrameTime();
	float turnAmount = this->turningSpeed * dt;
	Transform* trans = getTransform(); // Store the reference locally to save on function calls.

	if (this->targetForward == trans->GetForward()) {
		this->isTurning = false;
		return;
	}

	float angle = glm::angle(this->targetForward, trans->GetForward());
	glm::vec3 axis = YAXIS;
	if (angle < PI) {
		axis = glm::cross(trans->GetForward(), this->targetForward);
	}

	if (axis == ZERO_VEC3) {
		this->isTurning = false;
		return;
	}

	if (angle > turnAmount) {
		getTransform()->Rotate(turnAmount, axis);
	}
	else {
		getTransform()->Rotate(angle, axis);
		this->isTurning = false;
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
	cameFrom[startCell] = nullptr;

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
			while (checked != nullptr) {
				outPath.push_front(checked);
				checked = cameFrom[checked];
			}
			this->simplifyPath(outPath);
			return gScores[current];
		}

		openSet.remove(current);
		closedSet.push_back(current);

		std::list<GridCell*> neighbors;
		SINGLETONS->GetGridManager()->GetNeighbors(current, neighbors);
		for (auto iter = neighbors.begin(); iter != neighbors.end(); ++iter) {
			if (SINGLETONS->GetGridManager()->Passable(current, *iter)) {
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

void GridNavigator::simplifyPath(std::list<GridCell*>& outPath) {
	if (outPath.size() == 0) { return; }
	std::list<GridCell*> simplePath;

	auto startIter = outPath.begin();
	auto nextIter = startIter;
	while (nextIter != outPath.end()) {
		// Trace the direct route to the target cell from opposite corners.
		std::list<GridCell*> touchedCells;
		SINGLETONS->GetGridManager()->TouchedCells(*startIter, *nextIter, touchedCells);

		// Check if any of the cells are blocked.
		bool isRouteClear = true;
		for (auto iter = touchedCells.begin(); iter != touchedCells.end(); ++iter) {
			if (!SINGLETONS->GetGridManager()->Passable(*startIter, *iter)) {
				isRouteClear = false;
				break;
			}
		}

		if (isRouteClear) {
			++nextIter;
		}
		else {
			startIter = nextIter;
			--startIter;
			simplePath.push_back(*startIter);
		}
	}
	simplePath.push_back(outPath.back());

	outPath.clear();
	while (simplePath.size() > 0) {
		outPath.push_back(simplePath.front());
		simplePath.pop_front();
	}
}
