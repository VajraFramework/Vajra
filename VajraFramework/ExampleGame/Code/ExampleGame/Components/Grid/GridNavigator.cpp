//
//  GridNavigator.cpp
//  Created by Matt Kaufmann on 12/12/13.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
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
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
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
	this->gameObjectRef = (GameObject*)this->GetObject();
	ASSERT(this->gameObjectRef->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");

	this->currentCell = nullptr;
	this->isTraveling = false;
	this->isTurning = false;
	this->movementSpeed = 1.0f;
	this->turningSpeed = 90.0f inRadians;
	this->maxNavigableUnitType = UNIT_TYPE_UNKNOWN;
	this->ignoreOccupantsForPathing = false;
	this->ignoreEverything = false;
	this->navigationEnabled = true;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_NAVIGATION_REFRESH, this->GetTypeId(), false);
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

void GridNavigator::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_GRID_NAVIGATION_REFRESH:
			if (this->navigationEnabled) {
				this->recalculatePath();
			}
			break;

		default:
			break;
	}
}

void GridNavigator::SetGridPosition(int x, int z) {
	GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(x, z);
	this->SetGridPosition(cell);
}

void GridNavigator::SetGridPosition(glm::vec3 loc) {
	GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(loc);
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

bool GridNavigator::SetDestination(int x, int z, bool ignoreCellOccupants/*= false*/) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(x, z);
	return SetDestination(goalCell, ignoreCellOccupants);
}

bool GridNavigator::SetDestination(glm::vec3 loc, bool ignoreCellOccupants/*= false*/) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(loc);
	return SetDestination(goalCell, ignoreCellOccupants);
}

bool GridNavigator::SetDestination(GridCell* cell, bool ignoreCellOccupants/*= false*/) {
	std::list<GridCell*> newPath;
	float dist = this->calculatePath(this->currentCell, cell, newPath, ignoreCellOccupants);
	if (dist > 0.0f) {
		this->currentPath = newPath;
		this->setNextWaypoint();
		this->SetIsTraveling(true);
		this->ignoreOccupantsForPathing = ignoreCellOccupants;
	}
	return (dist >= 0.0f);
}

bool GridNavigator::AddDestination(int x, int z) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(x, z);
	return AddDestination(goalCell);
}

bool GridNavigator::AddDestination(glm::vec3 loc) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(loc);
	return AddDestination(goalCell);
}

bool GridNavigator::AddDestination(GridCell* cell) {
	if (this->currentPath.size() == 0) {
		return SetDestination(cell);
	}

	float dist = this->calculatePath(this->currentCell, cell, this->currentPath);
	if (dist > 0.0f) {
		this->SetIsTraveling(true);
	}
	return (dist >= 0.0f);
}

void GridNavigator::SetForcedDestination(int gridX, int gridZ) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(gridX, gridZ);
	this->SetForcedDestination(goalCell);
}

void GridNavigator::SetForcedDestination(glm::vec3 worldPosition) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(worldPosition);
	this->SetForcedDestination(goalCell);
}

void GridNavigator::SetForcedDestination(GridCell* cell) {
	this->currentPath.clear();
	this->currentPath.push_back(this->currentCell);
	this->AddForcedDestination(cell);
	this->setNextWaypoint();
	this->SetIsTraveling(true);
	this->ignoreOccupantsForPathing = true;
}

void GridNavigator::AddForcedDestination(int gridX, int gridZ) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(gridX, gridZ);
	this->AddForcedDestination(goalCell);
}

void GridNavigator::AddForcedDestination(glm::vec3 worldPosition) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(worldPosition);
	this->AddForcedDestination(goalCell);
}

void GridNavigator::AddForcedDestination(GridCell* cell) {
	this->currentPath.push_back(cell);
}

void GridNavigator::SetLookTarget(int x, int z) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(x, z);
	if (goalCell != nullptr) {
		SetLookTarget(goalCell->center);
	}
}

void GridNavigator::SetLookTarget(glm::vec3 loc) {
	glm::vec3 target = loc - this->getTransform()->GetPositionWorld();
	target.y = 0.0f;
	this->SetTargetForward(target);
}

void GridNavigator::SetLookTarget(glm::quat orient) {
	glm::vec3 target = QuaternionForwardVector(orient);
	target.y = 0.0f;
	this->SetTargetForward(target);
}

void GridNavigator::SetTargetForward(glm::vec3 forward) {
	if (forward != ZERO_VEC3) {
		forward = glm::normalize(forward);
		float angle = glm::angle(forward, this->getTransform()->GetForward());

		if (angle > ROUNDING_ERROR) {
			this->targetForward = forward;
			this->isTurning = true;
		}
	}
#ifdef DEBUG
	else {
		FRAMEWORK->GetLogger()->dbglog("Trying to set forward vector to zero");
	}
#endif
}

bool GridNavigator::CanReachDestination(int cellX, int cellZ, float maxDistance/*= -1.0f*/, bool ignoreCellOccupants/*= false*/) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(cellX, cellZ);
	return CanReachDestination(goalCell, maxDistance, ignoreCellOccupants);
}

bool GridNavigator::CanReachDestination(glm::vec3 worldPos, float maxDistance/*= -1.0f*/, bool ignoreCellOccupants/*= false*/) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(worldPos);
	return CanReachDestination(goalCell, maxDistance, ignoreCellOccupants);
}

bool GridNavigator::CanReachDestination(GridCell* cell, float maxDistance/*= -1.0f*/, bool ignoreCellOccupants/*= false*/) {
	std::list<GridCell*> dummyPath;
	float distance = this->calculatePath(this->currentCell, cell, dummyPath, ignoreCellOccupants);

	// If maxDist is negative, the path can be any length
	if (maxDistance < 0.0f) {
		return (distance >= 0.0f);
	}
	return ((distance >= 0.0f) && (distance <= maxDistance));
}

void GridNavigator::DisableNavigation() {
	this->navigationEnabled = false;
}

void GridNavigator::EnableNavigation() {
	this->navigationEnabled = true;
}

void GridNavigator::HaltMovement() {
	this->SetIsTraveling(false);
	this->isTurning = false;
	this->currentPath.clear();
	this->targetForward = ZERO_VEC3;
}

void GridNavigator::ReturnToCellCenter() {
	GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(this->GetObject()->GetComponent<Transform>()->GetPositionWorld());
	if (cell != nullptr) {
		this->currentPath.clear();
		this->currentPath.push_back(cell);
		this->SetIsTraveling(true);
	}
}

void GridNavigator::update() {
	if (this->navigationEnabled) {
		if (this->isTraveling) {
			followPath();
		}
		if (this->isTurning) {
			updateFacing();
		}
	}
}

void GridNavigator::recalculatePath() {
	glm::vec3 pos = this->gameObjectRef->GetTransform()->GetPositionWorld();
	GridCell* actualCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(pos);
	int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(pos.y);
	if (!this->canNavigateThroughCellAtElevation(actualCell, elevation, this->ignoreOccupantsForPathing)) {
		// If the navigator isp in an impossible area, get to the nearest walkable area.
		goToNearestPassableCell();
	}
	else if (this->isTraveling) {
		// If our current path is now blocked but another one is available, we'll change course.
		// If no valid path exists anymore, continue along current route.
		GridCell* destCell = this->currentPath.back();
		this->SetDestination(destCell, this->ignoreOccupantsForPathing);
	}
}

void GridNavigator::followPath() {
	float dt = ENGINE->GetTimer()->GetDeltaFrameTime();
	float distToTravel = this->movementSpeed * dt;
	Transform* trans = getTransform(); // Store the reference locally to save on function calls.

	glm::vec3 worldPosition = trans->GetPositionWorld();
	glm::vec3 tempLocation = worldPosition;

	int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(worldPosition.y);

	while ((distToTravel > ROUNDING_ERROR) && (this->currentPath.size() > 0)) {
		// Determine the target location.
		glm::vec3 targetLocation = this->currentPath.front()->center;
		// Prevent movement along y-axis.
		targetLocation.y = tempLocation.y;

		float distToTarget = glm::distance(tempLocation, targetLocation);
		if (distToTravel < distToTarget) {
			// Travel as far as possible towards the target location.
			float ratio = distToTravel / distToTarget;
			lerp(targetLocation, tempLocation, targetLocation, ratio);
		}
		GridCell* targetCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(targetLocation);

		// Make sure all of the cells along the path are clear.
		GridCell* tempCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(tempLocation);
		if (!this->ignoreEverything) {
			while ((this->currentSegment.size() > 0) && (tempCell != targetCell)) {
				if (this->canNavigateThroughCellAtElevation(this->currentSegment.front(), elevation, this->ignoreOccupantsForPathing)) {
					tempCell = this->currentSegment.front();
					this->currentSegment.pop_front();
				}
				else {
					// Stop once we find a cell that is blocked.
					break;
				}
			}
		}
		else {
			tempCell = targetCell;
		}

		// Can we actually make it to our intended location?
		if (tempCell == targetCell) {
			// If we can make it there, move the unit along that path.
			tempLocation = targetLocation;
			if (distToTravel >= distToTarget) {
				this->setNextWaypoint();
			}
			distToTravel -= distToTarget;
		}
		else {
			// If the path to our intended target was blocked, update the target but don't move this time.
			this->currentPath.clear();
			this->currentPath.push_back(tempCell);
			this->currentSegment.clear();
			SINGLETONS->GetGridManager()->GetGrid()->TouchedCells(tempLocation, tempCell->center, this->currentSegment);
		}
	}

	trans->Translate(tempLocation - worldPosition);
	GridCell* newCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(tempLocation);
	if (newCell != this->currentCell) {
		this->changeCell(newCell);
	}

	if (this->currentPath.size() > 0) {
		SetLookTarget(this->currentPath.front()->center);
	}
	else {
		this->SetIsTraveling(false);
		this->ignoreEverything = false;

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
	Transform* trans = this->GetObject()->GetComponent<Transform>();
	glm::vec3 pos = trans->GetPositionWorld();

	// Send a message to the GridManager "asking" to move from one cell to another.
	MessageChunk cellChangeMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	cellChangeMessage->SetMessageType(MESSAGE_TYPE_GRID_CELL_CHANGED);
	cellChangeMessage->messageData.iv1.x = goalCell->x;
	cellChangeMessage->messageData.iv1.y = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(pos.y);
	cellChangeMessage->messageData.iv1.z = goalCell->z;
	ENGINE->GetMessageHub()->SendMulticastMessage(cellChangeMessage, this->GetObject()->GetId());
}

float GridNavigator::calculatePath(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outPath, bool ignoreCellOccupants/*= false*/) {
	if ((startCell == nullptr) || (goalCell == nullptr)) {
		return -1.0f;
	}

	int startX = startCell->x;
	int startZ = startCell->z;
	int goalX = goalCell->x;
	int goalZ = goalCell->z;

	// Short-circuit if the start and end points are the same.
	if ((startX == goalX) && (startZ == goalZ)) { return 0.0f; }

	// Short-circuit if the elevations of the two cells are different.
	float y = this->GetObject()->GetComponent<Transform>()->GetPositionWorld().y;
	int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(y);
	if (!SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(goalCell->x, goalCell->z, elevation)) {
		return -1.0f;
	}

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
			this->simplifyPath(outPath, ignoreCellOccupants);
			return gScores[current];
		}

		openSet.remove(current);
		closedSet.push_back(current);

		std::list<GridCell*> neighbors;
		SINGLETONS->GetGridManager()->GetGrid()->GetNeighborCells(neighbors, current);
		for (auto iter = neighbors.begin(); iter != neighbors.end(); ++iter) {
			GridCell* neighbor = *iter;
			auto closedIter = std::find(closedSet.begin(), closedSet.end(), neighbor);
			if (closedIter != closedSet.end()) { continue; }

			if (this->canNavigateThroughCellAtElevation(neighbor, elevation, ignoreCellOccupants)) {
				auto openIter = std::find(openSet.begin(), openSet.end(), neighbor);
				float gScoreTentative = gScores[current] + actualTravelCost(current, neighbor);

				if ((openIter == openSet.end()) || (gScoreTentative < gScores[neighbor])) {
					cameFrom[neighbor] = current;
					gScores[neighbor] = gScoreTentative;
					fScores[neighbor] = gScoreTentative + travelCostEstimate(neighbor, goalCell);
					if (openIter == openSet.end()) {
						openSet.push_back(neighbor);
					}
				}
			}
		}
	}

	return -1.0f; // No path found
}

float GridNavigator::travelCostEstimate(GridCell* startCell, GridCell* goalCell) {
	// Just return the vector distance between the two cells
	return SINGLETONS->GetGridManager()->GetGrid()->GetGroundDistanceBetweenCells(startCell, goalCell);
}

float GridNavigator::actualTravelCost(GridCell* startCell, GridCell* goalCell) {
	// The travel cost is the vector distance between the two cells
	return SINGLETONS->GetGridManager()->GetGrid()->GetGroundDistanceBetweenCells(startCell, goalCell);
}

bool GridNavigator::canNavigateThroughCellAtElevation(GridCell* cell, int elevation, bool ignoreCellOccupants/*= false*/) {
	// Is the cell passable at all?
	if (SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(cell->x, cell->z, elevation)) {
		if (!ignoreCellOccupants) {

			// Does the cell have an occupant?
			ObjectIdType occId = cell->GetOccupantIdAtElevation(elevation);
			GameObject* occupant = ENGINE->GetSceneGraph3D()->GetGameObjectById(occId);
			if (occupant == nullptr) {
				return true;
			}

			// Does the occupant of the cell block this navigator?
			BaseUnit* occUnit = occupant->GetComponent<BaseUnit>();
			if (occUnit != nullptr) {
				UnitType otherType = occUnit->GetUnitType();
				return (otherType <= this->maxNavigableUnitType);
				//return true;
			}

		}
		return true;
	}

	return false;
}

void GridNavigator::simplifyPath(std::list<GridCell*>& outPath, bool ignoreCellOccupants/*= false*/) {
	if (outPath.size() > 0) {
		Transform* trans = this->gameObjectRef->GetTransform();
		int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(trans->GetPositionWorld().y);

		auto startIter = outPath.begin();  // Iterator to first cell along current path segment
		auto safeIter = startIter;         // Iterator to last known passable cell
		auto nextIter = startIter;         // Iterator to next cell to be checked
		nextIter++;

		while (nextIter != outPath.end()) {
			// Trace the direct route to the target cell.
			std::list<GridCell*> touchedCells;
			SINGLETONS->GetGridManager()->GetGrid()->TouchedCells(*startIter, *nextIter, touchedCells);

			// Check if any of the cells are blocked.
			bool isRouteClear = true;
			for (auto iter = touchedCells.begin(); iter != touchedCells.end(); ++iter) {
				if (*iter != *startIter) {
					if (!this->canNavigateThroughCellAtElevation(*iter, elevation, ignoreCellOccupants)) {
						isRouteClear = false;
						break;
					}
				}
			}

			if (isRouteClear) {
				if (safeIter != startIter) {
					safeIter = outPath.erase(safeIter);
				}
				else {
					++safeIter;
				}
				++nextIter;
			}
			else {
				startIter = safeIter;
			}
		}
	}
}

void GridNavigator::setNextWaypoint() {
	this->currentPath.pop_front();
	this->currentSegment.clear();
	if (this->currentPath.size() > 0) {
		Transform* trans = this->gameObjectRef->GetTransform();
		GridCell* cell = this->currentPath.front();
		SINGLETONS->GetGridManager()->GetGrid()->TouchedCells(trans->GetPositionWorld(), cell->center, this->currentSegment);
	}
}

void GridNavigator::goToNearestPassableCell() {
	glm::vec3 pos = this->gameObjectRef->GetTransform()->GetPositionWorld();
	GridCell* actualCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(pos);
	int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(pos.y);
	const float MAX_SEARCH_DISTANCE = 3.0f;
	std::list<GridCell*> neighbors;
	SINGLETONS->GetGridManager()->GetGrid()->GetNeighborCells(neighbors, actualCell, MAX_SEARCH_DISTANCE);

	for (auto iter = neighbors.begin(); iter != neighbors.end(); ++iter) {
		if (this->canNavigateThroughCellAtElevation(*iter, elevation, this->ignoreOccupantsForPathing)) {
			this->SetForcedDestination(*iter);
			this->ignoreEverything = true;
			break;
		}
	}
}

void GridNavigator::SetIsTraveling(bool isTraveling_) {
	if (this->isTraveling != isTraveling_) {

		this->isTraveling = isTraveling_;

	}
}

void GridNavigator::SetMaxNavigableUnitType(UnitType uType) {
	this->maxNavigableUnitType = uType;
}
