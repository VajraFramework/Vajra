#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSetCreationHelper/ShaderSetCreationHelper.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/Settings/Settings.h"
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

static std::map<std::string /* variable name */, bool> preprocessorVariables;

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

bool allTheSurroundingIfdefsPass(std::vector<bool>& ifdefs) {
	for (bool ifdef : ifdefs) {
		if (!ifdef) {
			return false;
		}
	}
	return true;
}

std::string CleanupShaderSourceForPreprocessorDirectives(std::string shaderSource) {
	std::string cleanedupShaderSource;
	std::istringstream iss(shaderSource);

	std::vector<bool> ifdefs;
	ifdefs.push_back(true);

	std::string line;
	while (iss.good()) {
		std::getline(iss, line);
		if (line.find("#") == std::string::npos || line.find("version") != std::string::npos) {
			if (allTheSurroundingIfdefsPass(ifdefs)) {
				cleanedupShaderSource += line + "\n";
			}

		} else {
			// Evaluate the preprocessor switch:
			line = StringUtilities::EraseStringFromString(line, "#");
			if (line.find("ifplatform") != std::string::npos) {
				line = StringUtilities::EraseStringFromString(line, "ifplatform");
				ifdefs.push_back(evaluatePlatformsLine(line));

			} else if (line.find("ifnplatform") != std::string::npos) {
				line = StringUtilities::EraseStringFromString(line, "ifnplatform");
				ifdefs.push_back(!evaluatePlatformsLine(line));

			} else if (line.find("ifdef") != std::string::npos) {
				line = StringUtilities::EraseStringFromString(line, "ifdef");
				ifdefs.push_back(evaluateIfdefLine(line));

			} else if (line.find("ifndef") != std::string::npos) {
				line = StringUtilities::EraseStringFromString(line, "ifndef");
				ifdefs.push_back(!evaluateIfdefLine(line));

			} else if (line.find("else") != std::string::npos) {
				bool topOfStack = ifdefs.back();
				ifdefs.pop_back();
				ifdefs.push_back(!topOfStack);

			} else if (line.find("endif") != std::string::npos) {
				ASSERT(!ifdefs.empty(), "Properly matched number of if / endif pairs");
				ifdefs.pop_back();

			} else {
				ASSERT(0, "Recognized preprocessor directive: %s", line.c_str());
			}
		}
	}

	ifdefs.pop_back();
	ASSERT(ifdefs.empty(), "Properly matched number of if / endif pairs");

	return cleanedupShaderSource;
}

void adjustPreprocessorVariablesFromSettings() {
	SettingLevel_t shadowmap_level = FRAMEWORK->GetSettings()->GetSetting(SETTING_TYPE_shadows);

	switch (shadowmap_level) {

	case SETTING_LEVEL_off: {
		ASSERT(preprocessorVariables.find("SHADOWMAPS_REALTIME") != preprocessorVariables.end(), "preprocessorVariables has and entry for SHADOWMAPS_REALTIME");
		preprocessorVariables["SHADOWMAPS_REALTIME"] = false;
	} break;

	case SETTING_LEVEL_low: {
		preprocessorVariables["SHADOWMAPS_REALTIME_NO_FILTERING"] = true;
		preprocessorVariables["SHADOWMAPS_REALTIME_POISSON_FILTERING"] = false;
		preprocessorVariables["SHADOWMAPS_REALTIME_PCF_FILTERING"] = false;
	} break;

	case SETTING_LEVEL_medium: {
		preprocessorVariables["SHADOWMAPS_REALTIME_NO_FILTERING"] = true;
		preprocessorVariables["SHADOWMAPS_REALTIME_POISSON_FILTERING"] = false;
		preprocessorVariables["SHADOWMAPS_REALTIME_PCF_FILTERING"] = false;
	} break;

	case SETTING_LEVEL_high: {
		preprocessorVariables["SHADOWMAPS_REALTIME_NO_FILTERING"] = false;
		preprocessorVariables["SHADOWMAPS_REALTIME_POISSON_FILTERING"] = false;
		preprocessorVariables["SHADOWMAPS_REALTIME_PCF_FILTERING"] = true;
	} break;

	default: {
	} break;
	}
}

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

	// Read the global settings from the settings file which can override this:
	adjustPreprocessorVariablesFromSettings();
}

bool IsPreprocessorVariableDefined(std::string variableName) {
	if (preprocessorVariables.find(variableName) != preprocessorVariables.end()) {
		return preprocessorVariables[variableName];
	}
	return false;
}


}

