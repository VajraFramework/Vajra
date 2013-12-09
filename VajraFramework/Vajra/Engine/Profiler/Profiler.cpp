//
//  Profiler.cpp
//  Created by Matt Kaufmann on 12/06/13.
//

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Profiler/Profiler.h"
#include "Vajra/Engine/Timer/Timer.h"

#include <cstdlib>

#ifdef DEBUG
unsigned long Profiler::seed = 0x21212121; // "!!!!"
#endif

Profiler::Profiler() {
}

Profiler::~Profiler() {
	destroy();
}

void Profiler::init() {

}

void Profiler::destroy() {
	this->experimentMap.clear();
}
#ifdef DEBUG
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
#endif
void Profiler::StartExperiment(std::string id) {
	auto experimentIter = this->experimentMap.find(id);

	if (experimentIter == this->experimentMap.end()) {
		this->experimentMap[id] = std::make_shared<ProfileExperiment>();
	}

	this->experimentMap[id]->Start();
}

void Profiler::StopExperiment(std::string id) {
	std::map<std::string, std::shared_ptr<ProfileExperiment>>::iterator experimentIter = this->experimentMap.find(id);

	if (experimentIter != this->experimentMap.end()) {
		this->experimentMap[id]->Stop();
	}
}

void Profiler::CloseExperiment(std::string id) {
	std::map<std::string, std::shared_ptr<ProfileExperiment>>::iterator experimentIter = this->experimentMap.find(id);

	if (experimentIter != this->experimentMap.end()) {
		this->experimentMap.erase(experimentIter);
	}
}

void Profiler::PrintLastExperimentLog(std::string id) {
	std::map<std::string, std::shared_ptr<ProfileExperiment>>::iterator experimentIter = this->experimentMap.find(id);
	if (experimentIter != this->experimentMap.end()) {
		experimentIter->second->PrintLastLog();
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("No data for experiment %s", id.c_str());
	}
}

void Profiler::PrintExperimentLogStats(std::string id) {
	std::map<std::string, std::shared_ptr<ProfileExperiment>>::iterator experimentIter = this->experimentMap.find(id);
	if (experimentIter != this->experimentMap.end()) {
		experimentIter->second->PrintLogStats();
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("No data for experiment %s", id.c_str());
	}
}

void Profiler::PrintExperimentLogStats(std::string id, std::ofstream& outputFile) {
	std::map<std::string, std::shared_ptr<ProfileExperiment>>::iterator experimentIter = this->experimentMap.find(id);
	if (experimentIter != this->experimentMap.end()) {
		experimentIter->second->PrintLogStats(outputFile);
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("No data for experiment %s", id.c_str());
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
	if (experimentIter != this->experimentMap.end()) {
		return experimentIter->second;
	}
	return nullptr;
}
