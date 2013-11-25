#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include <Vajra/Framework/OpenGL/ShaderSet/ShaderSetCreationHelper/ShaderSetCreationHelper.h>
#include "Vajra/Utilities/Utilities.h"

#include <fstream>
#include <map>
#include <string>


namespace ShaderSetCreationHelper {

#define SHADER_VARIABLES_FILE_NAME "shader_variables"

static bool gInited = false;
static std::map<std::string /* variableName */, std::string /* variable declaration */> gVariableDeclarations;
static std::map<std::string /* variableName */, std::string /* variable qualifier */> gVariableQualifiers;
static std::map<std::string /* variableName */, std::string /* variable datatype */> gVariableDatatypes;

static void initShaderSetCreationHelper() {
    std::string path = FRAMEWORK->GetFileSystemUtils()->GetDeviceShaderResourcesPath() +
    				   SHADER_VARIABLES_FILE_NAME;

    std::ifstream variablesFile(path.c_str());
    VERIFY_LOG(variablesFile.good(), "Successfully opened shader variables file %s for reading", path.c_str());

    while (variablesFile.good()) {
		std::string qualifier, datatype, variableName;
    	variablesFile >> qualifier >> datatype >> variableName;
    	if (qualifier != "" && datatype != "" && variableName != "") {
			ASSERT(gVariableDeclarations.find(variableName) == gVariableDeclarations.end(), "Not duplicate variable %s", variableName.c_str());
			ASSERT(gVariableQualifiers.find(variableName) == gVariableQualifiers.end(), "Not duplicate variable %s", variableName.c_str());
			ASSERT(gVariableDatatypes.find(variableName) == gVariableDatatypes.end(), "Not duplicate variable %s", variableName.c_str());

			std::string declaration = qualifier + " " + datatype + " " + variableName + ";";
			gVariableDeclarations[variableName] = declaration;
			gVariableQualifiers[variableName] = qualifier;
			gVariableDatatypes[variableName] = datatype;
    	}
    }

    FRAMEWORK->GetLogger()->dbglog("\nShader Variables Available:\n");
    for (auto it = gVariableDeclarations.begin(); it != gVariableDeclarations.end(); ++it) {
    	std::string declaration = it->second;
		FRAMEWORK->GetLogger()->dbglog("\n%s", declaration.c_str());
    }
    FRAMEWORK->GetLogger()->dbglog("\n\n");
}


std::string GetVariableDeclarationForVariableName(std::string variableName) {
	if (!gInited) {
		initShaderSetCreationHelper();
		gInited = true;
	}

	ASSERT(gVariableDeclarations.find(variableName) != gVariableDeclarations.end(), "Shader variable name found in variable declarations: %s", variableName.c_str());
	return gVariableDeclarations[variableName];
}

std::string GetVariableQualifierForVariableName(std::string variableName) {
	if (!gInited) {
		initShaderSetCreationHelper();
		gInited = true;
	}

	ASSERT(gVariableQualifiers.find(variableName) != gVariableQualifiers.end(), "Shader variable name found in variable qualifiers: %s", variableName.c_str());
	return gVariableQualifiers[variableName];
}

std::string GetVariableDatatypeForVariableName(std::string variableName) {
	if (!gInited) {
		initShaderSetCreationHelper();
		gInited = true;
	}

	ASSERT(gVariableDatatypes.find(variableName) != gVariableDatatypes.end(), "Shader variable name found in variable datatypes: %s", variableName.c_str());
	return gVariableDatatypes[variableName];
}

}
