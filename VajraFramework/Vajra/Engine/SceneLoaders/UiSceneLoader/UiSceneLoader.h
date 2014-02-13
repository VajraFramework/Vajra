#ifndef UISCENELOADER_H
#define UISCENELOADER_H

#include "Vajra/Engine/SceneLoaders/UiSceneLoader/ParserTags.h"
#include <string>
// Forward Declarations:
class UiTouchHandlers;

namespace UiSceneLoader {
	void UnloadCurrentUiScene();
	void LoadUiSceneFromUiSceneFile(std::string filePath, UiTouchHandlers* touchHandlers);
};

#endif // UISCENELOADER_H
