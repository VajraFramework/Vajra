#include <algorithm>

#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Utilities/Utilities.h"

Input::Input() {
	this->init();
}

Input::~Input() {
	this->destroy();
}

void Input::init() {
	_numTouches = 0;
}

void Input::destroy() {
}

bool touchOver(const Touch &touch)
{
	return (touch.phase == TouchPhase::Ended || touch.phase == TouchPhase::Cancelled);
}

void Input::updateInput() { 
	_frameTouches = _asyncTouches;
	_asyncTouches.erase( std::remove_if(_asyncTouches.begin(), _asyncTouches.end(), touchOver), _asyncTouches.end());
	for(std::vector<Touch>::iterator it = _asyncTouches.begin(); it != _asyncTouches.end(); ++it)
    {
    	it->phase = TouchPhase::Stationary;
    }
	//logTouches();

}

Touch Input::GetTouch(int index)
{
	ASSERT(index < _frameTouches.size(), "Index is greater than current number of touches");
	return _frameTouches[index];
}

void Input::AddTouch(int uId, float startX, float startY, TouchPhase phase) {
	
	Touch t;
	t.uId = uId;
	t.pos.x = startX;
	t.pos.y = startY;
	t.phase = phase;
	_asyncTouches.push_back(t);
	
    _numTouches++;
	//this->UpdateTouch(uId, startX, startY, TouchPhase::Begin);
}

void Input::UpdateTouch(int uId, float curX, float curY, TouchPhase phase) {
	//printf("Update Touch Called");
    //AddTouch(uId, curX, curY, phase);
    for(std::vector<Touch>::iterator it = _asyncTouches.begin(); it != _asyncTouches.end(); ++it)
    {
    	if(it->uId == uId)
    	{
    		it->pos.x = curX;
    		it->pos.y = curY;
    		it->phase = phase;
    		//_asyncTouches.push_back(*it);
    		break;
    	}
    }

}

void Input::logTouches() {

	printf("TOUCH LOG \n");
	for(std::vector<Touch>::iterator it = _frameTouches.begin(); it != _frameTouches.end(); ++it)
    {
    	printf("Touch pos: (%f, %f) %i \n", it->pos.x, it->pos.y, (int)it->phase);
    }
}