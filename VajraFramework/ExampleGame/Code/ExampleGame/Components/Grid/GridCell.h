//
//  GridCell.h
//  Created by Matt Kaufmann on 12/10/13.
//

#ifndef GRIDCELL_H
#define GRIDCELL_H

#include "ExampleGame/Components/Grid/GridConstants.h"
#include "Vajra/Engine/GameObject/GameObject.h"

#include "Libraries/glm/glm.hpp"

class GridCell {
public:
	GridCell(int x0, int y0, int z0, glm::vec3 origin0, glm::vec3 center0);
	~GridCell();

	ObjectIdType GetFirstOccupantId();
	ObjectIdType GetOccupantIdAtElevation(int elevation);
	ObjectIdType GetTopOccupantId(); // In other words, the last occupant in the list

	void SetFirstOccupantId(ObjectIdType id);
	void SetOccupantIdAtElevation(ObjectIdType id, int elevation);

	void AddZoneToCell(ObjectIdType id);
	void RemoveZoneFromCell(ObjectIdType id);

	ObjectIdType GetFirstZoneId();
	void GetZoneIds(std::list<ObjectIdType>& ids);

	int x; // Grid cell coordinate x
	int y; // Ground level elevation
	int z; // Grid cell coordinate z
	glm::vec3 origin; // Southwest corner of the cell in world coordinates
	glm::vec3 center; // Center of the cell in world coordinates
	ObjectIdType unitIds[NUM_ELEVATIONS];    // units occupying the cell at each elevation
	std::vector<ObjectIdType> staticObjs;    // list of static object IDs that occupy the cell
	std::list<ObjectIdType> zones;           // List of GridZones that touch this cell

	/*
	? Vision blocking
	? Cover
	? Illumination
	? Terrain Weight (for pathfinding)
	*/
};

#endif // GRIDCELL_H
