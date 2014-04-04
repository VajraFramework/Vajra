#ifndef SAVED_DATA_MANAGER_H
#define SAVED_DATA_MANAGER_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Framework/SavedDataManager/Bundle.h"

#include <map>
#include <string>

class SavedDataManager : public Object {
public:
	~SavedDataManager();

	bool    HasBundle(std::string bundleName);
	Bundle* CreateNewBundle(std::string bundleName);
	Bundle* GetSavedBundle (std::string bundleName);

private:
	SavedDataManager();
	void init();
	void destroy();

	// Utility Functions:
	void readAllSavedBundlesFromDisk();
	void writeListOfBundlesToDisk();

	std::map<std::string /* bundle name */, Bundle*> savedBundles;

	friend class Framework;
};

#endif // SAVED_DATA_MANAGER_H
