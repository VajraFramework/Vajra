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

	int westBound;
	int eastBound;
	int southBound;
	int northBound;

private:
	void init();
	void destroy();

	void updateVisualizer();
	void handleCameraModeChanged();

	GameObject* gameObjectRef;
	GameObject* visualizerObjectRef;

	bool displayVisualizerInGameMode;
	bool displayVisualizerInOverviewMode;

	static unsigned int componentTypeId;
};

#endif // GRIDZONE_H
