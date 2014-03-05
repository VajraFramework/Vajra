//
//  GridZone.h
//  Created by Matt Kaufmann on 01/07/14.
//

#ifndef GRIDZONE_H
#define GRIDZONE_H

#include "ExampleGame/Components/Grid/GridZones/GridZoneDeclarations.h"
#include "Vajra/Common/Components/Component.h"

//[[COMPONENT]]//
class GridZone : public Component {
public:
	GridZone();
	GridZone(Object* object_);
	~GridZone();

	static inline ComponentIdType GetTypeId() { return componentTypeId; };

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void GetZoneBounds(int& west, int& east, int& south, int& north);

	//[[PROPERTY]]//
	virtual void SetZoneBounds(int xMin, int zMin, int xMax, int zMax);

	//[[PROPERTY]]//
	void Visualize(bool shouldVisualize, std::string imageName, bool displayInGamemode, bool displayInOverviewMode);

	bool IsCellWithinZone(GridCell* cell);

protected:
	GridZoneType zoneType;

	// Coordinates of the zone's center
	int centerX, centerZ;

	// Rough direction of the object's facing
	glm::vec3 facing;

	// Stored as relative to object position
	int relativeWestBound;
	int relativeEastBound;
	int relativeSouthBound;
	int relativeNorthBound;

	// Stored as absolute grid indices
	int trueWestBound;
	int trueEastBound;
	int trueSouthBound;
	int trueNorthBound;

private:
	void init();
	void destroy();

	bool updateCenterPoint();  // Return true if center point has changed.
	bool updateFacing();       // Return true if facing has changed.
	void updateZoneBounds();
	void sendCellEvents();
	void onTransformChanged();
	void updateVisualizer();
	void handleCameraModeChanged();

	GameObject* gameObjectRef;
	GameObject* visualizerObjectRef;

	bool displayVisualizerInGameMode;
	bool displayVisualizerInOverviewMode;

	static unsigned int componentTypeId;
};

#endif // GRIDZONE_H
