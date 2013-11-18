#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/ModelLoader/Declarations.h"
#include "Vajra/Framework/DeviceUtils/ModelLoader/ModelLoader.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

namespace ModelLoader {

void LoadMeshFromModelFile(const char* filePath,
		std::vector<glm::vec3>&        outPositions,
		std::vector<glm::vec3>&        outNormals,
		std::vector<glm::vec2>&        outTexCoords,
		std::vector<unsigned int>&     outIndices,
		glm::vec3&                     outInitialPosition,
		glm::vec3&                     outInitialRotation,
		glm::vec3&                     outInitialScale,
		std::string&                   outTextureFilePath) {

	FRAMEWORK->GetLogger()->dbglog("\nLoading mesh data from model at %s", filePath);

	std::ifstream modelFile(filePath);
	VERIFY(modelFile.is_open(), "Successfully opened model file at %s", filePath);

	{
		int modelFormatVersionNumber = -1;
		modelFile >> modelFormatVersionNumber;
		VERIFY(modelFormatVersionNumber == MODEL_FORMAT_VERSION_NUMBER, "Model format version number (%d) matches expected (%d)", modelFormatVersionNumber, MODEL_FORMAT_VERSION_NUMBER);
	}

	{
		std::string modelName;
		modelFile >> modelName;
		FRAMEWORK->GetLogger()->dbglog("\nModel name: %s", modelName.c_str());
	}

	{
		modelFile >> outInitialPosition.x >> outInitialPosition.y >> outInitialPosition.z;
		modelFile >> outInitialRotation.x >> outInitialRotation.y >> outInitialRotation.z;
		modelFile >> outInitialScale.x >> outInitialScale.y >> outInitialScale.z;
	}

	{
		std::string meshName;
		modelFile >> meshName;
		FRAMEWORK->GetLogger()->dbglog("\nMesh name: %s", meshName.c_str());
	}

	{
		int numPositions;
		modelFile >> numPositions;
		while ((numPositions--) > 0) {
			glm::vec3 position;
			modelFile >> position.x;
			modelFile >> position.y;
			modelFile >> position.z;
			outPositions.push_back(position);
		}
	}

	{
		int numNormals;
		modelFile >> numNormals;
		while ((numNormals--) > 0) {
			glm::vec3 normal;
			modelFile >> normal.x;
			modelFile >> normal.y;
			modelFile >> normal.z;
			outNormals.push_back(normal);
		}
	}

	{
		int numTexCoords;
		modelFile >> numTexCoords;
		while ((numTexCoords--) > 0) {
			glm::vec2 texcoord;
			modelFile >> texcoord.x;
			modelFile >> texcoord.y;
			outTexCoords.push_back(texcoord);
		}
	}

	{
		int numTriangleFaces;
		modelFile >> numTriangleFaces;
		while ((numTriangleFaces--) > 0) {
			unsigned int a, b, c;
			modelFile >> a;
			modelFile >> b;
			modelFile >> c;
			outIndices.push_back(a);
			outIndices.push_back(b);
			outIndices.push_back(c);
		}
	}

	{
		std::string textureImageName;
		modelFile >> textureImageName;
		outTextureFilePath = FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesPath() + textureImageName;
	}

	FRAMEWORK->GetLogger()->dbglog("\nDone processing model file %s", filePath);
}

}
