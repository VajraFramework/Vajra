#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <vector>

// Forward Declarations:
class Camera;
class DirectionalLight;
class RenderLists;

namespace RenderScene {

void SetupStuff();
void CleanupStuff();

void RenderScene(RenderLists* renderLists, Camera* camera);
void RenderScene(RenderLists* renderLists, Camera* camera, DirectionalLight* directionalLight, std::vector<DirectionalLight*> additionalLights);

};

#endif // RENDER_SCENE_H
