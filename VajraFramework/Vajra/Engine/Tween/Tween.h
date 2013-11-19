#ifndef TWEEN_H
#define TWEEN_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Utilities/MathUtilities.h"

#include <map>
#include <string>

// Forward Declarations:
class GameObject;

class Tween : public Object {
public:
	~Tween();

	void TweenPosition(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition, float time);
	void TweenPosition(ObjectIdType gameObjectId, glm::vec3 finalPosition, float time);

	bool IsTweening(ObjectIdType gameObjectId);

private:
	Tween();
	void init();
	void destroy();

	// Utilitiy Functions:
	void tweenPosition_internal(GameObject* gameObject, glm::vec3 initialPosition, glm::vec3 finalPosition, float time);

	std::map<ObjectIdType, std::string /* tweenClipName */> ongoingTweens;

	friend class Engine;
};

#endif // TWEEN_H
