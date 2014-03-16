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

	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_START, this->GetTypeId(), false);
}

void GridNavigator::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void GridNavigator::start() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_NAVIGATION_REFRESH, this->GetTypeId(), false);
}

GridCell* GridNavigator::GetDestination() {
	if (this->currentPath.size() == 0) {
		return nullptr;
	}
	return SINGLETONS->GetGridManager()->GetGrid()->GetCell(this->currentPath.back());
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
		myTransform->SetPositionWorld(cell->center);
	}
	if (this->currentCell != cell) {
		this->changeCell(cell);
	}
}

bool GridNavigator::SetDestination(int x, int z, bool ignoreCellOccupants/*= false*/) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(x, z);
	return SetDestination(goalCell, ignoreCellOccupants);
}

bool GridNavigator::SetDestination(glm::vec3 loc, bool ignoreCellOccupants/*= false*/) {
	std::list<glm::vec3> newPath;
	float dist = this->calculatePath(this->gameObjectRef->GetTransform()->GetPositionWorld(), loc, newPath, ignoreCellOccupants);
	if (dist > 0.0f) {
		this->currentPath = newPath;
		this->setNextWaypoint();
		this->SetIsTraveling(true);
		this->ignoreOccupantsForPathing = ignoreCellOccupants;
	}
	return (dist >= 0.0f);
}

bool GridNavigator::SetDestination(GridCell* cell, bool ignoreCellOccupants/*= false*/) {
	if (cell != nullptr) {
		return this->SetDestination(cell->center, ignoreCellOccupants);
	}
	return false;
}

bool GridNavigator::AddDestination(int x, int z) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(x, z);
	return AddDestination(goalCell);
}

bool GridNavigator::AddDestination(glm::vec3 loc) {
	if (this->currentPath.size() == 0) {
		return SetDestination(loc);
	}

	float dist = this->calculatePath(this->gameObjectRef->GetTransform()->GetPositionWorld(), loc, this->currentPath);
	if (dist > 0.0f) {
		this->SetIsTraveling(true);
	}
	return (dist >= 0.0f);
}

bool GridNavigator::AddDestination(GridCell* cell) {
	if (cell != nullptr) {
		return this->AddDestination(cell->center);
	}
	return false;
}

void GridNavigator::SetForcedDestination(int gridX, int gridZ) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(gridX, gridZ);
	this->SetForcedDestination(goalCell);
}

void GridNavigator::SetForcedDestination(glm::vec3 worldPosition) {
	this->currentPath.clear();
	this->currentPath.push_back(this->gameObjectRef->GetTransform()->GetPositionWorld());
	this->AddForcedDestination(worldPosition);
	this->setNextWaypoint();
	this->SetIsTraveling(true);
	this->ignoreOccupantsForPathing = true;
}

void GridNavigator::SetForcedDestination(GridCell* cell) {
	if (cell != nullptr) {
		this->SetForcedDestination(cell->center);
	}
}

void GridNavigator::AddForcedDestination(int gridX, int gridZ) {
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(gridX, gridZ);
	this->AddForcedDestination(goalCell);
}

void GridNavigator::AddForcedDestination(glm::vec3 worldPosition) {
	this->currentPath.push_back(worldPosition);
}

void GridNavigator::AddForcedDestination(GridCell* cell) {
	if (cell != nullptr) {
		this->AddDestination(cell->center);
	}
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
		// Don't use glm's vector angle function because it never returns an angle greater than PI/2
		float angle = acos(glm::clamp(glm::dot(forward, this->getTransform()->GetForward()), -1.0f, 1.0f));

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
	if (cell != nullptr) {
		std::list<glm::vec3> dummyPath;
		float distance = this->calculatePath(this->gameObjectRef->GetTransform()->GetPositionWorld(), cell->center, dummyPath, ignoreCellOccupants);

		// If maxDist is negative, the path can be any length
		if (maxDistance < 0.0f) {
			return (distance >= 0.0f);
		}
		return ((distance >= 0.0f) && (distance <= maxDistance));
	}
	return false;
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
		this->currentPath.push_back(cell->center);
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
		this->SetDestination(this->currentPath.back(), this->ignoreOccupantsForPathing);
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
		glm::vec3 targetLocation = this->currentPath.front();
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
			this->currentPath.push_back(tempLocation);
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
		SetLookTarget(this->currentPath.front());
	}
	else {
		this->SetIsTraveling(false);
		this->ignoreEverything = false;
	}
}

void GridNavigator::updateFacing() {
	float dt = ENGINE->GetTimer()->GetDeltaFrameTime();
	float turnAmount = this->turningSpeed * dt;
	Transform* trans = getTransform(); // Store the reference locally to save on function calls.

	glm::vec3 currForward = QuaternionForwardVector(trans->GetOrientationWorld());
	// Don't use glm's vector angle function because it never returns an angle greater than PI/2
	float angle = acos(glm::clamp(glm::dot(this->targetForward, currForward), -1.0f, 1.0f));
	if (angle < ROUNDING_ERROR) {
		this->isTurning = false;
		return;
	}

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
	int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(pos.y);

	int gridX, gridZ;
	if (goalCell != nullptr) {
		gridX = goalCell->x;
		gridZ = goalCell->z;
	}
	else {
		gridX = -1;
		gridZ = -1;
	}

	// Send a message to the GridManager "asking" to move from one cell to another.
	MessageChunk cellChangeMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	cellChangeMessage->SetMessageType(MESSAGE_TYPE_GRID_CELL_CHANGED);
	cellChangeMessage->messageData.iv1.x = gridX;
	cellChangeMessage->messageData.iv1.y = elevation;
	cellChangeMessage->messageData.iv1.z = gridZ;
	ENGINE->GetMessageHub()->SendMulticastMessage(cellChangeMessage, this->GetObject()->GetId());
}

float GridNavigator::calculatePath(glm::vec3 startPosition, glm::vec3 goalPosition, std::list<glm::vec3>& outPath, bool ignoreCellOccupants/* = false*/) {
	GridCell* startCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(startPosition);
	GridCell* goalCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(goalPosition);

	if ((startCell == nullptr) || (goalCell == nullptr)) {
		return -1.0f;
	}

	int startX = startCell->x;
	int startZ = startCell->z;
	int goalX = goalCell->x;
	int goalZ = goalCell->z;

	// Short-circuit if the start and end points are the same.
	if ((startX == goalX) && (startZ == goalZ)) {
		outPath.push_back(goalPosition);
		return glm::distance(startPosition, goalPosition);
	}

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
	fScores[startCell] = travelCostEstimate(startPosition, goalPosition);
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
				outPath.push_front(checked->center);
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
				float gScoreTentative = gScores[current] + actualTravelCost(current->center, neighbor->center);

				if ((openIter == openSet.end()) || (gScoreTentative < gScores[neighbor])) {
					cameFrom[neighbor] = current;
					gScores[neighbor] = gScoreTentative;
					fScores[neighbor] = gScoreTentative + travelCostEstimate(neighbor->center, goalCell->center);
					if (openIter == openSet.end()) {
						openSet.push_back(neighbor);
					}
				}
			}
		}
	}

	return -1.0f; // No path found
}

float GridNavigator::travelCostEstimate(glm::vec3 startPosition, glm::vec3 goalPosition) {
	return glm::distance(startPosition, goalPosition);
}

float GridNavigator::actualTravelCost(glm::vec3 startPosition, glm::vec3 goalPosition) {
	return glm::distance(startPosition, goalPosition);
}

bool GridNavigator::canNavigateThroughCellAtElevation(GridCell* cell, int elevation, bool ignoreCellOccupants/*= false*/) {
	// Is the cell passable at all?
	if (SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(cell->x, cell->z, elevation)) {
		if (!ignoreCellOccupants) {

			// Does the cell have an occupant?
			ObjectIdType occId = cell->GetOccupantIdAtElevation(elevation);
			if(occId != OBJECT_ID_INVALID) {
				GameObject* occupant = ENGINE->GetSceneGraph3D()->GetGameObjectById(occId);
				if (occupant == nullptr) {
						return true;
				}

				// Does the occupant of the cell block this navigator?
				BaseUnit* occUnit = occupant->GetComponent<BaseUnit>();
				if (occUnit != nullptr) {
					UnitType otherType = occUnit->GetUnitType();
					return (otherType <= this->maxNavigableUnitType);
				}
			}

		}
		return true;
	}

	return false;
}

void GridNavigator::simplifyPath(std::list<glm::vec3>& outPath, bool ignoreCellOccupants/* = false*/) {
	if (outPath.size() > 0) {
		int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(outPath.front().y);

		auto startIter = outPath.begin();  // Iterator to first cell along current path segment
		auto safeIter = startIter;         // Iterator to last known passable point
		auto nextIter = startIter;         // Iterator to next point to be checked
		nextIter++;

		while (nextIter != outPath.end()) {
			// Trace the direct route to the target cell.
			std::list<GridCell*> touchedCells;
			SINGLETONS->GetGridManager()->GetGrid()->TouchedCells(*startIter, *nextIter, touchedCells);
			GridCell* startCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(*startIter);

			// Check if any of the cells are blocked.
			bool isRouteClear = true;
			for (auto iter = touchedCells.begin(); iter != touchedCells.end(); ++iter) {
				if (*iter != startCell) {
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
		SINGLETONS->GetGridManager()->GetGrid()->TouchedCells(trans->GetPositionWorld(), this->currentPath.front(), this->currentSegment);
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

		if (!isTraveling_) {
			// Send an event message to the unit
			ObjectIdType myId = this->GetObject()->GetId();
			Transform* trans = this->gameObjectRef->GetTransform();
			MessageChunk reachedDestinationMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			reachedDestinationMessage->SetMessageType(MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION);
			reachedDestinationMessage->messageData.fv1 = trans->GetPositionWorld();
			ENGINE->GetMessageHub()->SendPointcastMessage(reachedDestinationMessage, myId, myId);
		}
	}
}

void GridNavigator::SetMaxNavigableUnitType(UnitType uType) {
	this->maxNavigableUnitType = uType;
}
