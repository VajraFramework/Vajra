//
//  Profiler.h
//  Created by Matt Kaufmann on 12/06/13.
//

#ifndef PROFILER_H
#define PROFILER_H

#include <fstream>
#include <map>
#include <memory>
#include <string>

class ProfileExperiment;

class Profiler {
public:
	~Profiler();

	std::string StartExperiment(); // Use this for one-shot logs that should not be stored internally
	void StartExperiment(std::string id);
	void StopExperiment(std::string id);
	void CloseExperiment(std::string id);
	void PrintLastExperimentLog(std::string id);
	void PrintExperimentLogStats(std::string id);
	void PrintExperimentLogStats(std::string id, std::ofstream outFile);
	void PrintAllExperimentData();
	void PrintAllExperimentData(std::ofstream& outFile);
	std::shared_ptr<ProfileExperiment> GetExperiment(std::string id);

private:
	Profiler();
	void init();
	void destroy();

	std::map<std::string, std::shared_ptr<ProfileExperiment>> experimentMap;

	friend class Engine;
};

class ProfileExperiment {
public:
	ProfileExperiment();
	ProfileExperiment(std::string strId);

	void Start();
	void Stop();
	void PrintLastLog();
	void PrintLogStats();
	void PrintLogStats(std::ofstream& outFile);

	inline double GetPreviousRunTime()       { return this->lastDuration; }
	inline double GetMinimumRunTime()        { return this->minDuration; }
	inline double GetMaximumRunTime()        { return this->maxDuration; }
	inline double GetAverageRunTime()        { return this->avgDuration; }
	inline unsigned long long GetRunCount()  { return this->count; }

private:
	void init();

	std::string id;

	double startTime;
	double endTime;

	double totalTime;
	double lastDuration;
	double minDuration;
	double maxDuration;
	double avgDuration;
	unsigned long long count; // Number of times this phase has been active
};

#endif // PROFILER_H
