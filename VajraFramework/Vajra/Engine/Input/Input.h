#ifndef INPUT_H
#define INPUT_H

#include "Vajra/Common/Objects/Object.h"
#include "Libraries/glm/glm.hpp"

#include <iostream>
#include <vector>

enum TouchPhase
{
	Begin,
	End,
	Moved,
	Stationary,
	Cancelled
};

struct Touch
{
public:
	int uId;
	float fingerId;
	glm::vec2 pos;
	TouchPhase phase;
};

class Input : public Object {

static const int MAX_TOUCHES = 11;

public:
	~Input();

	Touch GetTouch(int index);
	void AddTouch(int uId, float startX, float startY);
	void UpdateTouch(int uId, float curX, float curY, TouchPhase phase);
	float GetTouchCount() { return _touches.size(); }
private:
	Input();
	void init();
	void destroy();

	void updateInput();
	void logTouches();
	int _numTouches = 0;

    std::vector<Touch> _touches;
	friend class Engine;
};

#endif // INPUT_H
