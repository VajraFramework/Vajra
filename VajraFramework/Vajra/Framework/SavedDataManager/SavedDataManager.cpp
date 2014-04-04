#include "Vajra/Framework/SavedDataManager/Bundle.h"
#include "Vajra/Framework/SavedDataManager/SavedDataManager.h"
#include "Vajra/Framework/SavedDataManager/SavedDataHelper.h"
#include "Vajra/Utilities/StringUtilities.h"
#include "Vajra/Utilities/Utilities.h"
#include "Vajra/Utilities/XmlParser/XmlParser.h"


SavedDataManager::SavedDataManager() {
}

SavedDataManager::~SavedDataManager() {
	this->destroy();
}

Bundle* SavedDataManager::CreateNewBundle(std::string bundleName) {
	ASSERT(this->savedBundles.find(bundleName) == this->savedBundles.end(), "Not duplicate bundle %s", bundleName.c_str());

	Bundle* bundle = new Bundle(bundleName);
	this->savedBundles[bundleName] = bundle;

	this->writeListOfBundlesToDisk();

	return bundle;
}

bool SavedDataManager::HasBundle(std::string bundleName) {
	return (this->savedBundles.find(bundleName) != this->savedBundles.end());
}

Bundle* SavedDataManager::GetSavedBundle(std::string bundleName) {
	ASSERT(this->savedBundles.find(bundleName) != this->savedBundles.end(), "Found bundle %s", bundleName.c_str());

	return this->savedBundles[bundleName];
}

#define BUNDLE_LIST_FILE_NAME "bundlelist"
#define BUNDLES_TAG "bundles"
#define BUNDLE_TAG "bundle"
#define NAME_ATTRIBUTE "name"

void SavedDataManager::readAllSavedBundlesFromDisk() {
	ASSERT(this->savedBundles.empty(), "Read all saved bundles only once per boot");

	std::ifstream bundleListFile;
	GetBundleFileHandleForReading(BUNDLE_LIST_FILE_NAME, bundleListFile);

	XmlParser* parser = new XmlParser();
	parser->ParseXmlFile(bundleListFile);

	XmlTree* tree = parser->GetXmlTree();
	VERIFY(tree != nullptr, "Got xml tree for bundle list");

	XmlNode* bundlesNode = tree->GetRootNode();
	VERIFY(bundlesNode != nullptr, "Got root node for bundle list");

	XmlNode* bundleNode = bundlesNode->GetFirstChildByNodeName(BUNDLE_TAG);
	while (bundleNode) {
		std::string bundleName = bundleNode->GetAttributeValueS(NAME_ATTRIBUTE);
		Bundle* newBundle = new Bundle(bundleName);
		newBundle->loadFromDisk();

		ASSERT(this->savedBundles.find(bundleName) == this->savedBundles.end(), "Not duplicate bundle %s", bundleName.c_str());
		this->savedBundles[bundleName] = newBundle;

		bundleNode = bundleNode->GetNextSiblingByNodeName(BUNDLE_TAG);
	}

	delete parser;
}

void SavedDataManager::writeListOfBundlesToDisk() {
	std::ofstream bundleListFile;
	GetBundleFileHandleForWriting(BUNDLE_LIST_FILE_NAME, bundleListFile);

	bundleListFile << "<" << BUNDLES_TAG << ">" << std::endl;
	for (std::map<std::string, Bundle*>::iterator it = this->savedBundles.begin(); it != this->savedBundles.end(); ++it) {
		std::string bundleName = it->first;
		bundleListFile << "<" << BUNDLE_TAG << " " << NAME_ATTRIBUTE << "=\"" << bundleName << "\">";
		bundleListFile << "</" << BUNDLE_TAG << ">" << std::endl;
	}
	bundleListFile << "</" << BUNDLES_TAG << ">";
}

void SavedDataManager::init() {
	this->readAllSavedBundlesFromDisk();
}

void SavedDataManager::destroy() {
}

