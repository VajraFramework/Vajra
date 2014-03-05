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

enum NumberTweenAffliationSceneGraph {
	NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D,
	NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui,
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

	void TweenToNumber(float fromNumber, float toNumber, float timePeriod, InterpolationType_t interpolationType, bool cancelCurrentTween, bool looping, bool continuousUpdates, std::string tweenName,
					   NumberTweenAffliationSceneGraph affiliation,
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

	void PausePostitionTween(ObjectIdType gameObjectId);
	void PauseOrientationTween(ObjectIdType gameObjectId);
	void PauseScaleTween(ObjectIdType gameObjectId);
	void PauseNumberTween(std::string tweenName);
	void PauseNumberTweensByAffiliation(NumberTweenAffliationSceneGraph affiliation);
	//
	void ResumePostitionTween(ObjectIdType gameObjectId);
	void ResumeOrientationTween(ObjectIdType gameObjectId);
	void ResumeScaleTween(ObjectIdType gameObjectId);
	void ResumeNumberTween(std::string tweenName);
	void ResumeNumberTweensByAffiliation(NumberTweenAffliationSceneGraph affiliation);

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

	void pauseOngoingTransformTween(ObjectIdType gameObjectId, TransformTweenTarget tweenTarget);
	void resumeOngoingTransformTween(ObjectIdType gameObjectId, TransformTweenTarget tweenTarget);
	//
	void pauseOngoingNumberTween(std::string tweenName);
	void resumeOngoingNumberTween(std::string tweenName);


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

	bool isPaused;

private:
	bool StepTween(float deltaTime);
	void ResetTween();
	//
	inline void Pause () { this->isPaused = true;  }
	inline void Resume() { this->isPaused = false; }

	OnGoingTransformTweenDetails() {
		this->currentTime = 0.0f;
		this->looping = false;
		this->callback = 0;
		this->isPaused = false;
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
	float currentTime;

	std::string tweenName;

	InterpolationType_t interpolationType;

	NumberTweenAffliationSceneGraph affiliation;

	void (*callback)(float fromNumber, float toNumber, float currentNumber, std::string tweenName, MessageData1S1I1F* userParams);
	// TODO [Hack] Switch this to be of base type MessageData when we have dynamic type checking
	MessageData1S1I1F* userParams;

	bool looping;

	bool continuousUpdates;

	bool isPaused;

private:
	bool StepTween(float deltaTime);
	void ResetTween();
	//
	inline void Pause () { this->isPaused = true;  }
	inline void Resume() { this->isPaused = false; }

	OnGoingNumberTweenDetails() {
		this->totalTime = this->fromNumber = this->toNumber = this->currentTime = 0.0f;
		this->interpolationType = INTERPOLATION_TYPE_LINEAR;
		this->continuousUpdates = false;
		this->looping = false;
		this->callback = 0;
		this->userParams = nullptr;
		this->isPaused = false;
	}


	friend class Tween;
};


#endif // TWEEN_H
