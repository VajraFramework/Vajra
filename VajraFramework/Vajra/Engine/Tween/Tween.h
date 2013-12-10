#ifndef TWEEN_H
#define TWEEN_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Utilities/MathUtilities.h"

#include <map>
#include <string>

// Forward Declarations:
class GameObject;
struct OnGoingTweenDetails;

class Tween : public Object {
public:
	~Tween();

	void TweenPosition(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition, float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);
	void TweenPosition(ObjectIdType gameObjectId, glm::vec3 finalPosition, float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);

	void TweenOrientation(ObjectIdType gameObjectId, glm::quat initialOrientation, glm::quat finalOrientation, float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);
	void TweenOrientation(ObjectIdType gameObjectId, glm::quat finalOrientation, float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);

	void TweenScale(ObjectIdType gameObjectId, glm::vec3 initialScale, glm::vec3 finalScale, float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);
	void TweenScale(ObjectIdType gameObjectId, glm::vec3 finalScale, float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);

	void TweenTransform(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition,
												   glm::quat initialOrientation, glm::quat finalOrientation,
												   glm::vec3 initialScale, glm::vec3 finalScale,
												   float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);

	bool IsTweening(ObjectIdType gameObjectId);

private:
	Tween();
	void init();
	void destroy();

	OnGoingTweenDetails* getOnGoingTweenDetails(ObjectIdType gameObjectId);

	// Utilitiy Functions:
	void tweenTransform_internal(GameObject* gameObject, glm::vec3 initialPosition, glm::vec3 finalPosition,
														glm::quat initialOrientation, glm::quat finalOrientation,
														glm::vec3 initialScale, glm::vec3 finalScale,
														float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName));

	std::map<ObjectIdType, OnGoingTweenDetails*> ongoingTweens;

	friend class Engine;
	friend class TweenCallbackComponent;
};

////////////////////////////////////////////////////////////////////////////////

struct OnGoingTweenDetails {
public:
	std::string tweenClipName;
	void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName);
private:
};

#endif // TWEEN_H
