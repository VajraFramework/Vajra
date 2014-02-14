//
//  LevelLoader.h
//  Created by Matt Kaufmann on 01/24/14.
//

#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Utilities/XmlParser/XmlParser.h"

#include <map>
#include <string>

class LevelLoader {
public:
	static void LoadLevelFromFile(std::string levelFilename);

private:
	static void loadStaticDataFromXml(XmlNode* staticNode);
	static void loadUnitDataFromXml  (XmlNode* unitBaseNode);
	static void loadOtherDataFromXml (XmlNode* otherDataNode);
	static void loadCameraDataFromXml(XmlNode* cameraNode);
	static void loadLinkDataFromXml  (XmlNode* linkBaseNode);

	static std::map<int, ObjectIdType> idsFromXml;
};

#endif // LEVELLOADER_H
