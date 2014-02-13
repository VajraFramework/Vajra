//
//  LevelLoader.h
//  Created by Matt Kaufmann on 01/24/14.
//

#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "Vajra/Utilities/XmlParser/XmlParser.h"

#include <string>

class LevelLoader {
public:
	static void LoadLevelFromFile(std::string levelFilename);
	static void LoadLevelsWithTutorials(std::vector<std::string>* /*levelsWithTutorials*/);
private:
	static void loadStaticDataFromXml(XmlNode* staticNode);
	static void loadUnitDataFromXml  (XmlNode* unitBaseNode);
	static void loadOtherDataFromXml (XmlNode* otherDataNode);
	static void loadCameraDataFromXml(XmlNode* cameraNode);

};

#endif // LEVELLOADER_H
