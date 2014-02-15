//
//  LevelLoader.h
//  Created by Matt Kaufmann on 01/24/14.
//

#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "Vajra/Utilities/XmlParser/XmlParser.h"

#include <string>

#define tutorialXmlPath "tutorials/tutorials.xml"

class LevelLoader {
public:
	static std::string LoadLevelFromFile(std::string levelFilename);
	static void LoadTutorialLevelNames(std::vector<std::string>* /*levelsWithTutorials*/);
	static void LoadTutorialData(std::string levelName);
private:
	static void loadStaticDataFromXml(XmlNode* staticNode);
	static void loadUnitDataFromXml  (XmlNode* unitBaseNode);
	static void loadOtherDataFromXml (XmlNode* otherDataNode);
	static void loadCameraDataFromXml(XmlNode* cameraNode);
};

#endif // LEVELLOADER_H
