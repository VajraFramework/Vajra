//
//  AiRoutine.h
//  Created by Matt Kaufmann on 01/15/14.
//

#ifndef AIROUTINE_H
#define AIROUTINE_H

#include "Vajra/Common/Components/Component.h"

#include <string>
#include <vector>

class AiRoutine : public Component {
public:
	AiRoutine();
	AiRoutine(Object* object_);
	~AiRoutine();

private:
	void init();
	void destroy();

	struct AiMarker {
		float ClockTick;
		glm::vec3 Position;
		glm::quat Orientation;
	};

	void parseTaskStrings();
	void processStart(int xPos, int zPos, int xLook, int zLook);
	void processWalk(int xDiff, int zDiff);
	void processLook(int xDiff, int zDiff);
	void processWait(float duration);

	void resumeSchedule();

	std::vector<std::string> taskStrings;
	std::vector<AiMarker> markers;
	int currentMarker;

	static ComponentIdType componentTypeId;

	friend class LevelManager;
};

#endif // AIROUTINE_H
