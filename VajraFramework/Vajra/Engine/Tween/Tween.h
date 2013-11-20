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

	bool IsTweening(ObjectIdType gameObjectId);

private:
	Tween();
	void init();
	void destroy();

	OnGoingTweenDetails* getOnGoingTweenDetails(ObjectIdType gameObjectId);

	// Utilitiy Functions:
	void tweenPosition_internal(GameObject* gameObject, glm::vec3 initialPosition, glm::vec3 finalPosition, float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) = 0);

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
