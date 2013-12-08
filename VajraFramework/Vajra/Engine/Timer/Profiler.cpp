//
//  Profiler.cpp
//  Created by Matt Kaufmann on 12/06/13.
//

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Timer/Profiler.h"
#include "Vajra/Engine/Timer/Timer.h"

#include <cstdlib>

Profiler::Profiler() {
	init();
}

Profiler::~Profiler() {
	destroy();
}

void Profiler::init() {

}

void Profiler::destroy() {
	this->experimentMap.clear();
}

unsigned long seed = 0x21212121; // "!!!!"
std::string Profiler::StartExperiment() {
	// Generate a four-character string id
	std::string experimentId;
	unsigned char* seedChars = (unsigned char*)&seed;

	experimentId += seedChars[0];
	experimentId += seedChars[1];
	experimentId += seedChars[2];
	experimentId += seedChars[3];

	StartExperiment(experimentId);

	seed++;
	if (seedChars[0] < 33) { seedChars[0] = 33; }
	if (seedChars[1] < 33) { seedChars[1] = 33; }
	if (seedChars[2] < 33) { seedChars[2] = 33; }
	if (seedChars[3] < 33) { seedChars[3] = 33; }

	return experimentId;
}

void Profiler::StartExperiment(std::string id) {
	std::map<std::string, std::shared_ptr<ProfileExperiment>>::iterator experimentIter = this->experimentMap.find(id);

	if (experimentIter == this->experimentMap.end()) {
		this->experimentMap[id] = std::make_shared<ProfileExperiment>();
		//ProfileExperiment* newExperiment = new ProfileExperiment(id);
		//this->experimentMap[id] = newExperiment;
	}

	this->experimentMap[id]->Start();
}

void Profiler::StopExperiment(std::string id) {
	std::map<std::string, std::shared_ptr<ProfileExperiment>>::iterator experimentIter = this->experimentMap.find(id);

	if (experimentIter == this->experimentMap.end()) { return; }

	this->experimentMap[id]->Stop();
}

void Profiler::CloseExperiment(std::string id) {
	std::map<std::string, std::shared_ptr<ProfileExperiment>>::iterator experimentIter = this->experimentMap.find(id);

	if (experimentIter == this->experimentMap.end()) { return; }

	this->experimentMap.erase(experimentIter);
}

void Profiler::PrintLastExperimentLog(std::string id) {
	std::map<std::string, std::shared_ptr<ProfileExperiment>>::iterator experimentIter = this->experimentMap.find(id);
	if (experimentIter == this->experimentMap.end()) {
		FRAMEWORK->GetLogger()->dbglog("No data for experiment %s", id.c_str());
	}
	else {
		experimentIter->second->PrintLogStats();
	}
}

void Profiler::PrintExperimentLogStats(std::string id) {
	std::map<std::string, std::shared_ptr<ProfileExperiment>>::iterator experimentIter = this->experimentMap.find(id);
	if (experimentIter == this->experimentMap.end()) {
		FRAMEWORK->GetLogger()->dbglog("No data for experiment %s", id.c_str());
	}
	else {
		experimentIter->second->PrintLogStats();
	}
}

void Profiler::PrintExperimentLogStats(std::string id, std::ofstream outFile) {
	std::map<std::string, std::shared_ptr<ProfileExperiment>>::iterator experimentIter = this->experimentMap.find(id);
	if (experimentIter == this->experimentMap.end()) {
		FRAMEWORK->GetLogger()->dbglog("No data for experiment %s", id.c_str());
	}
	else {
		experimentIter->second->PrintLogStats(outFile);
	}
}

void Profiler::PrintAllExperimentData() {
	for (auto iter = this->experimentMap.begin(); iter != this->experimentMap.end(); ++iter) {
		iter->second->PrintLogStats();
	}
}

void Profiler::PrintAllExperimentData(std::ofstream& outFile) {
	for (auto iter = this->experimentMap.begin(); iter != this->experimentMap.end(); ++iter) {
		iter->second->PrintLogStats(outFile);
	}
}

std::shared_ptr<ProfileExperiment> Profiler::GetExperiment(std::string id) {
	std::map<std::string, std::shared_ptr<ProfileExperiment>>::iterator experimentIter = this->experimentMap.find(id);
	if (experimentIter == this->experimentMap.end()) {
		return nullptr;
	}
	return experimentIter->second;
}

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
	this->lastDuration = duration;
	this->totalTime += this->lastDuration;
	if (this->lastDuration < this->minDuration) { this->minDuration = this->lastDuration; }
	if (this->lastDuration > this->maxDuration) { this->maxDuration = this->lastDuration; }
	this->avgDuration = this->totalTime / this->count;
}

void ProfileExperiment::PrintLastLog() {
	FRAMEWORK->GetLogger()->dbglog("\nProfile experiment %s: %f ms", this->id.c_str(), this->lastDuration * 1000.0);
}

void ProfileExperiment::PrintLogStats() {
	FRAMEWORK->GetLogger()->dbglog("\nStats for profile experiment %s: %f-%f ms over %llu runs (average %f ms)",
			this->id.c_str(), this->minDuration * 1000.0, this->maxDuration * 1000.0, this->count, this->avgDuration * 1000.0);
}

void ProfileExperiment::PrintLogStats(std::ofstream& outFile) {
	outFile << "Stats for profile experiment " << this->id << ": " << (this->minDuration * 1000.0) << "-" << (this->maxDuration * 1000.0)
			<< " ms over " << this->count << " runs (average " << (this->avgDuration * 1000.0) << " ms)"
			<< std::endl;
}
