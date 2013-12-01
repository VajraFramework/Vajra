#ifndef ARMATURE_H
#define ARMATURE_H

#include "Vajra/Common/Components/Component.h"

#include "Libraries/glm/glm.hpp"

#include <map>
#include <string>
#include <vector>

// Forward Declarations:
class Bone;
class Object;

class Armature : public Component {
public:
	Armature();
	Armature(Object* object_);
	~Armature();

	static inline unsigned int GetTypeId() { return componentTypeId; }

	void InitArmature(std::string armatureFilePath);

	// @Override
	virtual void HandleMessage(Message* message);

	void AddBone(Bone* newBone);
	Bone* GetBoneById(unsigned int boneId);
	Bone* GetBoneByName(std::string boneName);

	void SetRootBoneByName(std::string rootBoneName);

private:
	void init();
	void destroy();

	void updateBoneMatrices();

	static unsigned int componentTypeId;

	std::map<unsigned int /* bone id */, Bone*> bones;
	Bone* rootBone;
};

#endif // ARMATURE_H
