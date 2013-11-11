#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "Libraries/glm/glm.hpp"

#include <fstream>
#include <vector>
#include <string>

namespace ModelLoader {

void LoadMeshFromModelFile(const char* filePath, std::vector<glm::vec3>&     outPositions,
												 std::vector<glm::vec3>&     outNormals,
												 std::vector<glm::vec2>&     outTexCoords,
												 std::vector<unsigned int>&  outIndices,
												 glm::vec3&                  outInitialPosition,
												 glm::vec3&                  outInitialRotation,
												 glm::vec3&                  outInitialScale,
												 std::string&                outTextureFilePath);

}

#endif // MODEL_LOADER_H
