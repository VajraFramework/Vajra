#ifndef TWEEN_H
#define TWEEN_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Utilities/MathUtilities.h"

#include <list>
#include <map>
#include <string>

// Forward Declarations:
class GameObject;
struct OnGoingNumberTweenDetails;
struct OnGoingTransformTweenDetails;

class Tween : public Object {
public:
	~Tween();

	void TweenPosition(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition, float time, bool looping = false, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);
	void TweenPosition(ObjectIdType gameObjectId, glm::vec3 finalPosition, float time, bool looping = false, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);

	void TweenOrientation(ObjectIdType gameObjectId, glm::quat initialOrientation, glm::quat finalOrientation, float time, bool looping = false, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);
	void TweenOrientation(ObjectIdType gameObjectId, glm::quat finalOrientation, float time, bool looping = false, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);

	void TweenScale(ObjectIdType gameObjectId, glm::vec3 initialScale, glm::vec3 finalScale, float time, bool looping = false, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);
	void TweenScale(ObjectIdType gameObjectId, glm::vec3 finalScale, float time, bool looping = false, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);

	void TweenTransform(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition,
												   glm::quat initialOrientation, glm::quat finalOrientation,
												   glm::vec3 initialScale, glm::vec3 finalScale,
												   float time, bool looping = false,
												   void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);

	void TweenToNumber(float fromNumber, float toNumber, float timePeriod, bool looping, bool continuousUpdates, std::string tweenName,
					   void (*callback)(float normalizedProgress, std::string tweenName));

	bool IsTweening(ObjectIdType gameObjectId);

private:
	Tween();
	void init();
	void destroy();

	void updateTweens();

	OnGoingTransformTweenDetails* getOnGoingTransformTweenDetails(ObjectIdType gameObjectId);

	// Utilitiy Functions:
	void tweenTransform_internal(GameObject* gameObject, glm::vec3 initialPosition, glm::vec3 finalPosition,
														glm::quat initialOrientation, glm::quat finalOrientation,
														glm::vec3 initialScale, glm::vec3 finalScale,
														float time, bool looping, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName));

	std::map<ObjectIdType, OnGoingTransformTweenDetails*> ongoingTransformTweens;
	std::list<OnGoingNumberTweenDetails*> ongoingNumberTweens;

	friend class Engine;
	friend class TweenCallbackComponent;
};

////////////////////////////////////////////////////////////////////////////////

struct OnGoingTransformTweenDetails {
public:
	std::string tweenClipName;
	void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName);

	bool looping;

private:
	OnGoingTransformTweenDetails() {
		this->looping = false;
	}
	friend class Tween;
};

struct OnGoingNumberTweenDetails {
public:
	float totalTime;

	float fromNumber;
	float toNumber;
	//
	float currentNumber;

	void (*callback)(float normalizedProgress, std::string tweenName);

	bool continuousUpdates;

	std::string tweenName;

	bool looping;

private:
	OnGoingNumberTweenDetails() {
		this->totalTime = this->fromNumber = this->toNumber = this->currentNumber = 0.0f;
		this->continuousUpdates = false;
		this->looping = false;
	}
	friend class Tween;
};

#endif // TWEEN_H
