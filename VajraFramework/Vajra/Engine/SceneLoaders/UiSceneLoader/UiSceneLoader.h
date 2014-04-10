#ifndef UISCENELOADER_H
#define UISCENELOADER_H

#include "Vajra/Engine/SceneLoaders/UiSceneLoader/ParserTags.h"

#include <string>

// Forward Declarations:
class UiTouchHandlers;

namespace UiSceneLoader {
	void UnloadCurrentUiScene();
	void LoadUiSceneFromUiSceneFile(std::string filePath, UiTouchHandlers* touchHandlers);
	void AdjustPositionForResolution(int& posX_pixels_out, int& posY_pixels_out, std::string x_wrto, std::string y_wrto, int width_pixels, int height_pixels, int intended_scene_width_pixels, int intended_scene_height_pixels);
};

#endif // UISCENELOADER_H
