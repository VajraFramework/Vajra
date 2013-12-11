#ifndef DEBUG_DRAWER_H
#define DEBUG_DRAWER_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/AssetLibrary/Assets/MeshAssets/MeshAsset.h"

#include "Libraries/glm/glm.hpp"

#include <memory>
#include <vector>


// Forward Declarations:
class Engine;
class GameObject;
class Transform;

namespace DebugDraw {
#ifdef DEBUG
void DrawPoint(glm::vec3 position);
void DrawCube(glm::vec3 position, float size);
void DrawLine(glm::vec3 from, glm::vec3 to);
void DrawLine(glm::vec3 from, glm::vec3 direction, float length);
#else
void DrawPoint(...)
#define DrawCube(...)
#define DrawLine(...)
#endif
}

class DebugDrawer : public Object {
public:
	~DebugDrawer();

private:
	DebugDrawer();
	void init();
	void destroy();

	void drawPoint(glm::vec3 position);
	void drawCube(glm::vec3 position, float size);
	void drawLine(glm::vec3 from, glm::vec3 to);

	Transform* identityTransform;
	Transform* generalPurposeTransform;

	// Friends:
	friend class Engine;
	//
	friend void DebugDraw::DrawPoint(glm::vec3 position);
	friend void DebugDraw::DrawCube(glm::vec3 position, float size);
	friend void DebugDraw::DrawLine(glm::vec3 from, glm::vec3 to);
	friend void DebugDraw::DrawLine(glm::vec3 from, glm::vec3 direction, float length);
};

#endif // DEBUG_DRAWER_H
