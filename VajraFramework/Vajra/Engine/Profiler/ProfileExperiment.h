//
//  Profiler.h
//  Created by Matt Kaufmann on 12/09/13.
//

#ifndef PROFILE_EXPERIMENT_H
#define PROFILE_EXPERIMENT_H

#include <fstream>
#include <string>

class ProfileExperiment {
public:
	ProfileExperiment();
	ProfileExperiment(std::string strId);

	inline double GetPreviousRunTime()       { return this->lastDuration; }
	inline double GetMinimumRunTime()        { return this->minDuration; }
	inline double GetMaximumRunTime()        { return this->maxDuration; }
	inline double GetAverageRunTime()        { return this->avgDuration; }
	inline unsigned long long GetRunCount()  { return this->count; }

private:
	void init();

	void Start();
	void Stop();
	void PrintLastLog();
	void PrintLogStats();
	void PrintLogStats(std::ofstream& outputFile);

	std::string id;

	double startTime;
	double endTime;

	double totalTime;
	double lastDuration;
	double minDuration;
	double maxDuration;
	double avgDuration;
	unsigned long long count; // Number of times this experiment was run

	friend class Profiler;
};

#endif // PROFILE_EXPERIMENT_H
