#ifndef UISCENELOADER_H
#define UISCENELOADER_H

#include "Vajra/Engine/SceneLoaders/UiSceneLoader/Declarations.h"

// Forward Declarations:
class UiTouchHandlers;

namespace UiSceneLoader {
	void LoadUiSceneFromUiSceneFile(const char* filePath, UiTouchHandlers* touchHandlers);
};

#endif // UISCENELOADER_H
