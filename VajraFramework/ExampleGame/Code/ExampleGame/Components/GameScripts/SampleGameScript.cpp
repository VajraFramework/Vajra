#include "ExampleGame/Components/GameScripts/SampleGameScript.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/MathUtilities.h"

SampleGameScript::SampleGameScript() : GameScript() {
	// TODO [Hack] call start from an event handle once that event is implemented
	this->start();
}

SampleGameScript::SampleGameScript(Object* object_) : GameScript(object_) {
	// TODO [Hack] call start from an event handle once that event is implemented
	this->start();
}

SampleGameScript::~SampleGameScript() {
	// TODO [Hack] call end from an event handle once that event is implemented
	this->end();
}

void SampleGameScript::start() {
	this->getTransform()->Scale(0.5f, 0.5f, 0.5f);
}

void SampleGameScript::end() {
}

void SampleGameScript::update() {
	if(ENGINE->GetInput()->GetTouchCount() > 0) {
		this->getTransform()->Rotate(5.0f, YAXIS);
	}
}
