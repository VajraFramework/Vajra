//
//  LevelLoader.h
//  Created by Matt Kaufmann on 01/24/14.
//

#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Utilities/XmlParser/XmlParser.h"

#include <map>
#include <string>

#define tutorialXmlPath "tutorials/tutorials.xml"
#define levelListXmlPath "levels/_levelList.xml"

class LevelLoader {
public:
	static std::string LoadLevelFromFile(std::string levelFilename);
	static void LoadLevelData(std::vector<LevelData>* /*levelData*/); 
	static void LoadTutorialData(std::string levelName);
private:

	static void LoadTutorialLevelNames(std::vector<std::string>* /*levelsWithTutorials*/);
	static void loadStaticDataFromXml(XmlNode* staticNode);
	static void loadUnitDataFromXml  (XmlNode* unitBaseNode);
	static void loadOtherDataFromXml (XmlNode* otherDataNode);
	static void loadCameraDataFromXml(XmlNode* cameraNode);
	static void loadLinkDataFromXml  (XmlNode* linkBaseNode);

	static LevelType stringToLevelType(std::string);

	static std::map<int, ObjectIdType> idsFromXml;
};

#endif // LEVELLOADER_H
