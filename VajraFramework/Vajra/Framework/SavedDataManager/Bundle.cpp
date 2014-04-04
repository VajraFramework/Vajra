#include "Vajra/Framework/SavedDataManager/Bundle.h"
#include "Vajra/Framework/SavedDataManager/SavedDataHelper.h"
#include "Vajra/Utilities/StringUtilities.h"
#include "Vajra/Utilities/Utilities.h"
#include "Vajra/Utilities/XmlParser/XmlParser.h"

#include <fstream>

Bundle::Bundle(std::string bundleName_) {
	this->init(bundleName_);
}

Bundle::~Bundle() {
	this->destroy();
}

bool Bundle::HasKey(std::string key) {
	return (this->data.find(key) != this->data.end());
}

int Bundle::GetInt(std::string key) {
	auto it = this->data.find(key);
	VERIFY(it != this->data.end(), "Found value for key %s", key.c_str());
	return StringUtilities::ConvertStringToInt(it->second);
}

float Bundle::GetFloat(std::string key) {
	auto it = this->data.find(key);
	VERIFY(it != this->data.end(), "Found value for key %s", key.c_str());
	return StringUtilities::ConvertStringToFloat(it->second);
}

bool Bundle::GetBool(std::string key) {
	auto it = this->data.find(key);
	VERIFY(it != this->data.end(), "Found value for key %s", key.c_str());
	return StringUtilities::ConvertStringToBool(it->second);
}

std::string Bundle::GetString(std::string key) {
	auto it = this->data.find(key);
	VERIFY(it != this->data.end(), "Found value for key %s", key.c_str());
	return (it->second);
}

void Bundle::PutInt(std::string key, int value) {
	this->data[key] = StringUtilities::ConvertIntToString(value);
}

void Bundle::PutFloat(std::string key, float value) {
	this->data[key] = StringUtilities::ConvertFloatToString(value);
}

void Bundle::PutBool(std::string key, bool value) {
	this->data[key] = StringUtilities::ConvertBoolToString(value);
}

void Bundle::PutString(std::string key, std::string value) {
	this->data[key] = value;
}

void Bundle::Save() {
	this->writeToDisk();
}

#define BUNDLE_TAG "bundle"
#define DATA_TAG "data"
#define KEY_ATTRIBUTE "key"
#define VALUE_ATTRIBUTE "value"

void Bundle::loadFromDisk() {
	std::ifstream file;
	GetBundleFileHandleForReading(this->bundleName, file);

	if (file.good()) {
		XmlParser* parser = new XmlParser();
		parser->ParseXmlFile(file);

		XmlTree* tree = parser->GetXmlTree();
		VERIFY(tree != nullptr, "Got xml tree for bundle data in bundle %s", this->bundleName.c_str());

		XmlNode* rootNode = tree->GetRootNode();
		VERIFY(rootNode != nullptr, "Got root node for bundle data in bundle %s", this->bundleName.c_str());
		VERIFY(rootNode->GetName() == BUNDLE_TAG, "Got bundle node for bundle data in bundle %s", this->bundleName.c_str());

		XmlNode* dataNode = rootNode->GetFirstChildByNodeName(DATA_TAG);
		while(dataNode != nullptr) {
			std::string key   = dataNode->GetAttributeValueS(KEY_ATTRIBUTE);
			std::string value = dataNode->GetAttributeValueS(VALUE_ATTRIBUTE);
			this->PutString(key, value);
			//
			dataNode = dataNode->GetNextSiblingByNodeName(DATA_TAG);
		}

		delete parser;
	}
}

void Bundle::writeToDisk() {
	std::ofstream file;
	GetBundleFileHandleForWriting(this->bundleName, file);

	file << "<" << BUNDLE_TAG << ">" << std::endl;

	for (std::map<std::string, std::string>::iterator it = this->data.begin(); it != this->data.end(); ++it) {
		file << "<" << DATA_TAG;
		std::string key   = it->first;
		std::string value = it->second;
		file << " " << KEY_ATTRIBUTE << "=\"" << key << "\" " << VALUE_ATTRIBUTE << "=\"" << value << "\">";
		file << "</" << DATA_TAG << ">" << std::endl;
	}

	file << "</" << BUNDLE_TAG << ">" << std::endl;
}

void Bundle::init(std::string bundleName_) {
	this->bundleName = bundleName_;
}

void Bundle::destroy() {
}
