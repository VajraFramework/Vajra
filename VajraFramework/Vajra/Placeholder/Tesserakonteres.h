#ifndef TESSERAKONTERES_H
#define TESSERAKONTERES_H

// Forward Declarations:
class Mesh;

#include <string>

namespace Tesserakonteres {

	void initGameObjectsForScene();
	void initModels(Mesh* out_mesh, std::string imageName);

};

#endif // TESSERAKONTERES_H
