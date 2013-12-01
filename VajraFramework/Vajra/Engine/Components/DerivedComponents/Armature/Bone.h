#ifndef BONE_H
#define BONE_H

#include "Libraries/glm/glm.hpp"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Declarations.h"

#include <string>
#include <vector>

// Forward Declarations:
class Armature;
class GameObject;

class Bone {
public:
	Bone();
	~Bone();

	inline unsigned int GetId() { return this->id; }

	std::string GetName();
	void SetName(std::string name);

	void SetBindPoseMatrixGlobal(glm::mat4 m);

	void Rotate(float angleInDegrees, glm::vec3 axis, bool boneSpace = false);
	void Translate(float distance, glm::vec3 along);

	void SetParent(Bone* parentBone);
	void AddChild(Bone* childBone);

private:
	void init();
	void destroy();

	void updateBoneMatrices();
	void rippleBoneMatrixUpdates();

	std::string name;
	unsigned int id;

	glm::mat4 bindPoseMatrixGlobal;
	//
	glm::mat4 toBoneMatrix;
	glm::mat4 toWorldMatrix;
	//
	glm::mat4 localRotationMatrix;
	glm::mat4 localTranslationMatrix;
	glm::mat4 localRotationMatrixCumulative;
	glm::mat4 localTranslationMatrixCumulative;

	Bone* parent;
	std::vector<Bone*> children;

	Armature* armature;

#if DRAW_BONES
	GameObject* visualizer;
#endif

	friend class Armature;
};

#endif // BONE_H
