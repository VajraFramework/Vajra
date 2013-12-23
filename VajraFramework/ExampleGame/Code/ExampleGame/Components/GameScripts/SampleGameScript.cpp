#include "ExampleGame/Components/GameScripts/SampleGameScript.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

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
	FRAMEWORK->GetLogger()->dbglog("\nSampleGameScript start");
}

void SampleGameScript::end() {
	FRAMEWORK->GetLogger()->dbglog("\nSampleGameScript end");
}

void SampleGameScript::update() {
	FRAMEWORK->GetLogger()->dbglog("\nSampleGameScript update");
	FRAMEWORK->GetLogger()->dbglog("\nTransform %d", this->getTransform());
	FRAMEWORK->GetLogger()->dbglog("\nMesh Renderer %d", this->getMeshRenderer());
}