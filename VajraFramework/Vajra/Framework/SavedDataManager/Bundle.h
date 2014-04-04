#ifndef BUNDLE_H
#define BUNDLE_H

#include <map>
#include <string>

class Bundle {
public:
	~Bundle();

	bool HasKey(std::string key);

	int         GetInt   (std::string key);
	float       GetFloat (std::string key);
	bool        GetBool  (std::string key);
	std::string GetString(std::string key);

	void PutInt   (std::string key, int         value);
	void PutFloat (std::string key, float       value);
	void PutBool  (std::string key, bool        value);
	void PutString(std::string key, std::string value);

	void Save();

private:
	Bundle(std::string bundleName_);
	void init(std::string bundleName_);
	void destroy();

	// Utility Functions:
	void loadFromDisk();
	void writeToDisk();

	std::string bundleName;

	std::map<std::string /* key */, std::string /* value */> data;

	friend class SavedDataManager;
};

#endif // BUNDLE_H
