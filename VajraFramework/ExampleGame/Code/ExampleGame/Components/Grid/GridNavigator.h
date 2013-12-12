//
//  GridNavigator.h
//  Created by Matt Kaufmann on 12/12/13.
//

#ifndef GRIDNAVIGATOR_H
#define GRIDNAVIGATOR_H

#include "Vajra/Common/Components/Component.h"

class GridNavigator : public Component {
public:
	GridNavigator();
	GridNavigator(Object* object_);
	~GridNavigator();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(Message* message);

private:
	void init();
	void destroy();

	static ComponentIdType componentTypeId;
};

#endif // GRIDNAVIGATOR_H
