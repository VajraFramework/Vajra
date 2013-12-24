#ifndef SCENEGRAPHUI_H
#define SCENEGRAPHUI_H

#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Utilities/Utilities.h"
#include "Vajra/Utilities/CommonDeclarations.h"

#include <map>

// Forward Declarations:
class Camera;
class DirectionalLight;

class SceneGraphUi : public SceneGraph {
public:
	~SceneGraphUi();

	Camera* GetMainCamera();
	void SetMainCameraId(ObjectIdType id);

	// @Override
	virtual void update();
	virtual void draw();

protected:
	SceneGraphUi();
	virtual void init();
	virtual void destroy();

	ObjectIdType mainCameraId;

	friend class Engine;
};

#endif // SCENEGRAPHUI_H
