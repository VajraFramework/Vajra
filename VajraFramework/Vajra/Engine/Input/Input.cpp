#include <algorithm>

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Utilities/Utilities.h"

Input::Input() {
	this->init();
}

Input::~Input() {
	this->destroy();
}

void Input::init() {
}

void Input::destroy() {
}

bool touchOver(const Touch &touch)
{
	return (touch.phase == TouchPhase::Ended || touch.phase == TouchPhase::Cancelled);
}

void Input::updateInput() { 
	this->frameTouches = this->asyncTouches;
	this->asyncTouches.erase( std::remove_if(this->asyncTouches.begin(), this->asyncTouches.end(), touchOver), this->asyncTouches.end());
	for(std::vector<Touch>::iterator it = this->asyncTouches.begin(); it != this->asyncTouches.end(); ++it)
    {
    	it->phase = TouchPhase::Stationary;
    }
	//logTouches();
}

Touch Input::GetTouch(int index) {
	ASSERT(index < this->frameTouches.size(), "Index is greater than current number of touches");
	return this->frameTouches[index];
}

void Input::AddTouch(int uId, float startX, float startY, TouchPhase phase) {
	Touch t;
	t.uId = uId;
	t.pos.x = startX;
	t.pos.y = startY;
	t.prevPos = t.pos;
	t.phase = phase;
	t.fingerId = this->asyncTouches.size();
	this->asyncTouches.push_back(t);
}

void Input::UpdateTouch(int uId, float curX, float curY, TouchPhase phase) {
	for(std::vector<Touch>::iterator it = this->asyncTouches.begin(); it != this->asyncTouches.end(); ++it)
    {
    	if(it->uId == uId)
    	{
    		it->prevPos = it->pos;
    		it->pos.x = curX;
    		it->pos.y = curY;
    		it->phase = phase;
    		break;
    	}
    }
}

void Input::logTouches() {
	printf("TOUCH LOG \n");
	for(std::vector<Touch>::iterator it = this->frameTouches.begin(); it != this->frameTouches.end(); ++it)
    {
    	printf("Touch id: %i pos: (%f, %f) %i \n", it->fingerId, it->pos.x, it->pos.y, (int)it->phase);
    }
}