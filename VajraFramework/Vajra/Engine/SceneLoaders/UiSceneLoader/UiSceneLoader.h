#ifndef UISCENELOADER_H
#define UISCENELOADER_H

#include "Vajra/Engine/SceneLoaders/UiSceneLoader/ParserTags.h"

// Forward Declarations:
class UiTouchHandlers;

namespace UiSceneLoader {
	void LoadUiSceneFromUiSceneFile(std::string filePath, UiTouchHandlers* touchHandlers);
};

#endif // UISCENELOADER_H
