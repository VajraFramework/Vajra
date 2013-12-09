//
//  Profiler.h
//  Created by Matt Kaufmann on 12/09/13.
//

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Timer/ProfileExperiment.h"
#include "Vajra/Engine/Timer/Profiler.h"
#include "Vajra/Engine/Timer/Timer.h"

ProfileExperiment::ProfileExperiment() : id("") {
	init();
}

ProfileExperiment::ProfileExperiment(std::string strId) : id(strId) {
	init();
}

void ProfileExperiment::init() {
	this->startTime = 0.0;
	this->endTime = 0.0;
	this->totalTime = 0.0;
	this->lastDuration = 0.0;
	this->minDuration = 0.0;
	this->maxDuration = 0.0;
	this->avgDuration = 0.0;
	this->count = 0;
}

void ProfileExperiment::Start() {
	this->startTime = ENGINE->GetTimer()->GetHighResAbsoluteTime();
	this->count++;
}

void ProfileExperiment::Stop() {
	this->endTime = ENGINE->GetTimer()->GetHighResAbsoluteTime();
	double duration = this->endTime - this->startTime;
	this->totalTime += duration;
	if (this->lastDuration < this->minDuration) { this->minDuration = duration; }
	if (this->lastDuration > this->maxDuration) { this->maxDuration = duration; }
	this->lastDuration = duration;
	this->avgDuration = this->totalTime / this->count;
}

void ProfileExperiment::PrintLastLog() {
	FRAMEWORK->GetLogger()->dbglog("\nProfile experiment %s: %f ms", this->id.c_str(), this->lastDuration * 1000.0);
}

void ProfileExperiment::PrintLogStats() {
	FRAMEWORK->GetLogger()->dbglog("\nStats for profile experiment %s: %f-%f ms over %llu runs (average %f ms)",
			this->id.c_str(), this->minDuration * 1000.0, this->maxDuration * 1000.0, this->count, this->avgDuration * 1000.0);
}

void ProfileExperiment::PrintLogStats(std::ofstream& outputFile) {
	outputFile << "Stats for profile experiment " << this->id << ": " << (this->minDuration * 1000.0) << "-" << (this->maxDuration * 1000.0)
			   << " ms over " << this->count << " runs (average " << (this->avgDuration * 1000.0) << " ms)"
			   << std::endl;
}
