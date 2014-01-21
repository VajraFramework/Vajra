#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSetCreationHelper/ShaderSetCreationHelper.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Utilities/StringUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <stack>
#include <vector>


namespace ShaderSetCreationHelper {

#define SHADER_VARIABLES_FILE_NAME "shader_variables"

static bool gInited = false;
static std::map<std::string /* variableName */, std::string /* variable declaration */> gVariableDeclarations;
static std::map<std::string /* variableName */, std::string /* variable qualifier */> gVariableQualifiers;
static std::map<std::string /* variableName */, std::string /* variable datatype */> gVariableDatatypes;

bool evaluatePlatformsLine(std::string line) {
	std::vector<std::string> targetPlatforms = StringUtilities::SplitStringIntoTokensOnDelimiter(line, '|', true);
	if (StringUtilities::FindStringInVectorOfStrings(targetPlatforms, FRAMEWORK->GetDeviceProperties()->GetOperatingSystem())) {
		return true;
	} else {
		return false;
	}
}

bool evaluateIfdefLine(std::string line) {
	std::string preprocessorVariable = StringUtilities::EraseStringFromString(line, " ");
	return ShaderSetCreationHelper::IsPreprocessorVariableDefined(preprocessorVariable);
}

static void initShaderSetCreationHelper() {
    std::string path = FRAMEWORK->GetFileSystemUtils()->GetDeviceShaderResourcesPath() +
    				   SHADER_VARIABLES_FILE_NAME;

    std::ifstream variablesFile(path.c_str());
    VERIFY_LOG(variablesFile.good(), "Successfully opened shader variables file %s for reading", path.c_str());

    while (variablesFile.good()) {
		std::string qualifier, datatype, variableName, arraySpecifier;

    	variablesFile >> qualifier >> datatype >> variableName;

    	if (variableName.find("[") != std::string::npos) {
    		arraySpecifier = variableName.substr(variableName.find("["));
    		variableName.erase(variableName.find("["));
    	}

    	if (qualifier != "" && datatype != "" && variableName != "") {
			ASSERT(gVariableDeclarations.find(variableName) == gVariableDeclarations.end(), "Not duplicate variable %s", variableName.c_str());
			ASSERT(gVariableQualifiers.find(variableName) == gVariableQualifiers.end(), "Not duplicate variable %s", variableName.c_str());
			ASSERT(gVariableDatatypes.find(variableName) == gVariableDatatypes.end(), "Not duplicate variable %s", variableName.c_str());

			std::string declaration = qualifier + " " + datatype + " " + variableName + arraySpecifier + ";";
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

std::string CleanupShaderSourceForPreprocessorDirectives(std::string shaderSource) {
	std::string cleanedupShaderSource;
	std::istringstream iss(shaderSource);

	std::stack<bool> ifdefs;
	ifdefs.push(true);

	std::string line;
	while (iss.good()) {
		std::getline(iss, line);
		if (line.find("#") == std::string::npos) {
			if (ifdefs.top()) {
				cleanedupShaderSource += line + "\n";
			}

		} else {
			// Evaluate the preprocessor switch:
			line = StringUtilities::EraseStringFromString(line, "#");
			if (line.find("ifplatform") != std::string::npos) {
				line = StringUtilities::EraseStringFromString(line, "ifplatform");
				ifdefs.push(evaluatePlatformsLine(line));

			} else if (line.find("ifnplatform") != std::string::npos) {
				line = StringUtilities::EraseStringFromString(line, "ifplatform");
				ifdefs.push(!evaluatePlatformsLine(line));

			} else if (line.find("ifdef") != std::string::npos) {
				line = StringUtilities::EraseStringFromString(line, "ifdef");
				ifdefs.push(evaluateIfdefLine(line));

			} else if (line.find("ifndef") != std::string::npos) {
				line = StringUtilities::EraseStringFromString(line, "ifdef");
				ifdefs.push(!evaluateIfdefLine(line));

			} else if (line.find("else") != std::string::npos) {
				bool topOfStack = ifdefs.top();
				ifdefs.pop();
				ifdefs.push(!topOfStack);

			} else if (line.find("endif") != std::string::npos) {
				ASSERT(!ifdefs.empty(), "Properly matched number of if / endif pairs");
				ifdefs.pop();
			}
		}
	}

	ifdefs.pop();
	ASSERT(ifdefs.empty(), "Properly matched number of if / endif pairs");

	return cleanedupShaderSource;
}

std::map<std::string /* variable name */, bool> preprocessorVariables;
void LoadPreprocessorVariables() {
	std::string filePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceShaderResourcesPath() + "preprocessor_variables";
	std::ifstream file(filePath.c_str());

	std::string buffer;
	while (file.good()) {
		std::getline(file, buffer);
		if (buffer.find("!") == std::string::npos) {
			preprocessorVariables[buffer] = true;
		} else {
			preprocessorVariables[StringUtilities::EraseStringFromString(buffer, "!")] = false;
		}
	}
	file.close();
}

bool IsPreprocessorVariableDefined(std::string variableName) {
	if (preprocessorVariables.find(variableName) != preprocessorVariables.end()) {
		return preprocessorVariables[variableName];
	}
	return false;
}


}

