#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <vector>

// Forward Declarations:
class Camera;
class DirectionalLight;
class RenderLists;

/*
 * Note that when using this to be able to see anything
 * the DRAWING_DEPTH_BUFFER_CONTENTS flag should also be set in the preprocessor_variables for the shaders
 */
// #define DRAWING_DEPTH_BUFFER_CONTENTS

namespace RenderScene {

void SetupStuff();
void CleanupStuff();

void RenderScene(RenderLists* renderLists, Camera* camera);
void RenderScene(RenderLists* renderLists, Camera* camera, DirectionalLight* directionalLight, std::vector<DirectionalLight*> additionalLights);

};

#endif // RENDER_SCENE_H
