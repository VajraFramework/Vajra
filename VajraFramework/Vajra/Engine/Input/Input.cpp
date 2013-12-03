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

void Input::updateInput() { 
	logTouches();
}

Touch Input::GetTouch(int index)
{
	ASSERT(index < _touches.size(), "Index is greater than current number of touches");
	return _touches[index];
}

void Input::AddTouch(int uId, float startX, float startY) {
	
	Touch t;
	t.uId = uId;
	t.pos.x = startX;
	t.pos.y = startY;

	_touches.push_back(t);
	
    _numTouches++;
	//this->UpdateTouch(uId, startX, startY, TouchPhase::Begin);
}

void Input::UpdateTouch(int uId, float curX, float curY, TouchPhase phase) {
    for(std::vector<Touch>::iterator it = _touches.begin(); it != _touches.end(); ++it)
    {
    	if(it->uId == uId)
    	{
    		it->pos.x = curX;
    		it->pos.y = curY;
    		it->phase = phase;
    		break;
    	}
    }
}

void Input::logTouches() {

	printf("Touch Pos Debug");
	for(std::vector<Touch>::iterator it = _touches.begin(); it != _touches.end(); ++it)
    {
    	printf("Touch pos: (%f, %f) \n", it->pos.x, it->pos.y);
    }
}