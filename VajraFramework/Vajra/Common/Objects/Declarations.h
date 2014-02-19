#ifndef OBJECT_DECLARATIONS_H
#define OBJECT_DECLARATIONS_H

typedef int ObjectIdType;

#define OBJECT_ID_INVALID 0

enum ClassType {
	CLASS_TYPE_OBJECT = 1,
	CLASS_TYPE_GAMEOBJECT = CLASS_TYPE_OBJECT << 1,
	CLASS_TYPE_UIOBJECT = CLASS_TYPE_OBJECT << 2,
	CLASS_TYPE_UIELEMENT = CLASS_TYPE_OBJECT << 3,
	NUM_CLASS_TYPES = CLASS_TYPE_OBJECT << 4,
};

#endif // OBJECT_DECLARATIONS_H
