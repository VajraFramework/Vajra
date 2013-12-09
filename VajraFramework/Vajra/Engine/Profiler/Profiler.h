//
//  Profiler.h
//  Created by Matt Kaufmann on 12/06/13.
//

#ifndef PROFILER_H
#define PROFILER_H

#include "Vajra/Engine/Profiler/ProfileExperiment.h"

#include <fstream>
#include <map>
#include <memory>
#include <string>

class Profiler {
public:
	~Profiler();

#ifdef DEBUG
	std::string StartExperiment(); // Use this for one-shot logs that should not be stored internally
#endif
	void StartExperiment(std::string id);
	void StopExperiment(std::string id);
	void CloseExperiment(std::string id);
	void PrintLastExperimentLog(std::string id);
	void PrintExperimentLogStats(std::string id);
	void PrintExperimentLogStats(std::string id, std::ofstream& outputFile);
	void PrintAllExperimentData();
	void PrintAllExperimentData(std::ofstream& outFile);
	std::shared_ptr<ProfileExperiment> GetExperiment(std::string id);

private:
	Profiler();
	void init();
	void destroy();

	std::map<std::string, std::shared_ptr<ProfileExperiment>> experimentMap;
#ifdef DEBUG
	static unsigned long seed; // ID used for auto-assign
#endif

	friend class Engine;
};

#endif // PROFILER_H
