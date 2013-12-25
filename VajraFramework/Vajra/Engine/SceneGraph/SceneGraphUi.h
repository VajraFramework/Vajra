#ifndef SCENEGRAPHUI_H
#define SCENEGRAPHUI_H

#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Utilities/Utilities.h"
#include "Vajra/Utilities/CommonDeclarations.h"

#include <map>

#define UI_SCREENWIDTH_PREFERRED  1024
#define UI_SCREENHEIGHT_PREFERRED 768

// Forward Declarations:
class Camera;
class DirectionalLight;

class SceneGraphUi : public SceneGraph {
public:
	~SceneGraphUi();

	// @Override
	virtual void update();
	virtual void draw();

protected:
	SceneGraphUi();
	virtual void init();
	virtual void destroy();

	friend class Engine;
};

#endif // SCENEGRAPHUI_H
