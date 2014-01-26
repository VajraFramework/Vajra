#ifndef INPUT_DEFINITIONS_H
#define INPUT_DEFINITIONS_H

#include "Libraries/glm/glm.hpp"

enum TouchPhase
{
	Began,
	Ended,
	Moved,
	Stationary,
	Cancelled
};

enum GestureState {
	GestureState_Inactive,
	GestureState_Start ,
	GestureState_Changed,
	GestureState_End,
	GestureState_Cancelled
};

// TODO [Implement] Change this to use pools of Touch objects
struct Touch {
private:
	int uId;
public:
	int fingerId;
	glm::vec2 pos;
	glm::vec2 prevPos;
	TouchPhase phase;

	friend class Input;
};

struct Pinch {
public:
	Pinch() {
		this->gestureState = GestureState::GestureState_Inactive;
	}

	float scale;
	float velocity;
	GestureState gestureState;
};

struct LongPress {
public:
	LongPress() {
		this->gestureState = GestureState::GestureState_Inactive;
	}

	glm::vec2 pos;
	GestureState gestureState;
};

#endif // INPUT_DEFINITIONS_H
