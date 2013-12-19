//
//  GridNavigator.cpp
//  Created by Matt Kaufmann on 12/12/13.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"

#include "Libraries/glm/glm.hpp"

#include "Vajra/Common/Messages/Declarations.h"
#include "Vajra/Common/Messages/Message.h"

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
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void GridNavigator::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void GridNavigator::HandleMessage(Message* message) {
	switch (message->GetMessageType()) {
		case MESSAGE_TYPE_FRAME_EVENT:
			update();
			break;

		default:
			break;
	}
}

void GridNavigator::update() {

}

float GridNavigator::calculatePath(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outPath) {
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
		//GridManager::GetNeighbors(current, neighbors);
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
