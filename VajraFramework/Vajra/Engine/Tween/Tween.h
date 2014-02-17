#ifndef TWEEN_H
#define TWEEN_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Utilities/MathUtilities.h"

#include <list>
#include <map>
#include <string>

// Forward Declarations:
struct OnGoingNumberTweenDetails;
struct OnGoingTransformTweenDetails;
class AnimationKeyFrame;
class GameObject;
class MessageData1S1I1F;

enum TransformTweenTarget {
	TRANSFORM_TWEEN_TARGET_POSITION,
	TRANSFORM_TWEEN_TARGET_ORIENTATION,
	TRANSFORM_TWEEN_TARGET_SCALE,
	TRANSFORM_TWEEN_TARGET_NUM_TARGETS
};

enum TweenTranslationCurveType {
	TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
	TWEEN_TRANSLATION_CURVE_TYPE_PARABOLA,
};

class Tween : public Object {
public:
	~Tween();

	void TweenPosition(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition, float time, bool cancelCurrentTween = true, TweenTranslationCurveType curveType = TWEEN_TRANSLATION_CURVE_TYPE_LINEAR, bool looping = false, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);
	void TweenPosition(ObjectIdType gameObjectId, glm::vec3 finalPosition, float time, bool cancelCurrentTween = true, TweenTranslationCurveType curveType = TWEEN_TRANSLATION_CURVE_TYPE_LINEAR, bool looping = false, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);

	void TweenOrientation(ObjectIdType gameObjectId, glm::quat initialOrientation, glm::quat finalOrientation, float time, bool cancelCurrentTween = true, bool looping = false, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);
	void TweenOrientation(ObjectIdType gameObjectId, glm::quat finalOrientation, float time, bool cancelCurrentTween = true, bool looping = false, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);

	void TweenScale(ObjectIdType gameObjectId, glm::vec3 initialScale, glm::vec3 finalScale, float time, bool cancelCurrentTween = true, bool looping = false, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);
	void TweenScale(ObjectIdType gameObjectId, glm::vec3 finalScale, float time, bool cancelCurrentTween = true, bool looping = false, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);

	void TweenToNumber(float fromNumber, float toNumber, float timePeriod, bool cancelCurrentTween, bool looping, bool continuousUpdates, std::string tweenName,
					   MessageData1S1I1F* userParams,
					   void (*callback)(float fromNumber, float toNumber, float currentNumber, std::string tweenName, MessageData1S1I1F* userParams));

	bool IsTweening_transform(ObjectIdType gameObjectId, TransformTweenTarget transformTweenTarget);
	bool IsTweening_number(std::string tweenName);

	OnGoingTransformTweenDetails* GetOnGoingTransformTweenDetails(ObjectIdType gameObjectId, TransformTweenTarget tweenTarget);
	OnGoingNumberTweenDetails*    GetOnGoingNumberTweenDetails(std::string tweenName);

	void CancelPostitionTween(ObjectIdType gameObjectId);
	void CancelOrientationTween(ObjectIdType gameObjectId);
	void CancelScaleTween(ObjectIdType gameObjectId);
	void CancelNumberTween(std::string tweenName);

private:
	Tween();
	void init();
	void destroy();

	void updateTweens();
	void updateNumberTweens(float deltaTime);
	void updateTransformTweens(float deltaTime);
	void updateTransformTweens_internal(float deltaTime, std::map<ObjectIdType, OnGoingTransformTweenDetails*>* ongoingTweens);

	// Utilitiy Functions:

	void cancelOngoingTransformTween(ObjectIdType gameObjectId, TransformTweenTarget tweenTarget);
	void cancelOngoingNumberTween(std::string tweenName);


	void createNewTransformTween(ObjectIdType gameObjectId, TransformTweenTarget transformTweenTarget, glm::vec3 from_v, glm::vec3 to_v,
														float time,
														void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName),
														bool looping,
														TweenTranslationCurveType curveType = TWEEN_TRANSLATION_CURVE_TYPE_LINEAR);
	void createNewTransformTween(ObjectIdType gameObjectId, TransformTweenTarget transformTweenTarget, glm::quat from_q, glm::quat to_q,
														float time,
														void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName),
														bool looping);


	std::map<ObjectIdType, OnGoingTransformTweenDetails*> ongoingPositionTweens;
	std::map<ObjectIdType, OnGoingTransformTweenDetails*> ongoingOrientationTweens;
	std::map<ObjectIdType, OnGoingTransformTweenDetails*> ongoingScaleTweens;

	std::map<std::string /* tweenName */, OnGoingNumberTweenDetails*> ongoingNumberTweens;

	friend class Engine;
};

////////////////////////////////////////////////////////////////////////////////


struct OnGoingTransformTweenDetails {
public:
	float totalTime;
	float currentTime;

	glm::vec3 from_v;
	glm::vec3 to_v;
	glm::vec3 current_v;

	glm::quat from_q;
	glm::quat to_q;
	glm::quat current_q;

	TransformTweenTarget tweenTarget;
	TweenTranslationCurveType curveType;

	ObjectIdType gameObjectId;

	std::string userDefinedTweenName;

	void (*callback)(ObjectIdType gameObjectId, std::string userDefinedTweenName);

	bool looping;

private:
	bool StepTween(float deltaTime);
	void ResetTween();

	OnGoingTransformTweenDetails() {
		this->currentTime = 0.0f;
		this->looping = false;
		this->callback = 0;
	}
	friend class Tween;
};

struct OnGoingNumberTweenDetails {
public:

	~OnGoingNumberTweenDetails();

	float totalTime;

	float fromNumber;
	float toNumber;
	//
	float currentNumber;

	std::string tweenName;
	void (*callback)(float fromNumber, float toNumber, float currentNumber, std::string tweenName, MessageData1S1I1F* userParams);
	// TODO [Hack] Switch this to be of base type MessageData when we have dynamic type checking
	MessageData1S1I1F* userParams;

	bool looping;

	bool continuousUpdates;

private:
	bool StepTween(float deltaTime);
	void ResetTween();

	OnGoingNumberTweenDetails() {
		this->totalTime = this->fromNumber = this->toNumber = this->currentNumber = 0.0f;
		this->continuousUpdates = false;
		this->looping = false;
		this->callback = 0;
		this->userParams = nullptr;
	}


	friend class Tween;
};


#endif // TWEEN_H
