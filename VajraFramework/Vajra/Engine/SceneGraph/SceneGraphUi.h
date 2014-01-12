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
class UiTouchManager;

class SceneGraphUi : public SceneGraph {
public:
	virtual ~SceneGraphUi();

	// @Override
	virtual void update();
	virtual void draw();

	// @Override
	virtual bool TestTouch(Touch touch);
	virtual void OnTouch(int touchIndex);

	UiTouchManager* GetUiTouchManager() { return this->uiTouchManager; }

protected:
	SceneGraphUi();
	virtual void init();
	virtual void destroy();

	UiTouchManager* uiTouchManager;
	void handleTouches();

	friend class Engine;
};

#endif // SCENEGRAPHUI_H
