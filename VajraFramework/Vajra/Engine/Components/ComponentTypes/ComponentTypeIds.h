#ifndef COMPONENT_TYPE_IDS_H
#define COMPONENT_TYPE_IDS_H

enum EngineComponentIdType {
	COMPONENT_TYPE_ID_INVALID = -1,
	COMPONENT_TYPE_ID_DEFAULT,
	COMPONENT_TYPE_ID_TRANSFORM,
	COMPONENT_TYPE_ID_CAMERA,
	COMPONENT_TYPE_ID_RENDERER,
	COMPONENT_TYPE_ID_DIRECTIONAL_LIGHT,
	COMPONENT_TYPE_ID_RIGID_ANIMATION,
	COMPONENT_TYPE_ID_BAKED_SKELETAL_ANIMATION,
	COMPONENT_TYPE_ID_UI_CALLBACK,
	COMPONENT_TYPE_ID_ARMATURE,
	COMPONENT_TYPE_ID_AUDIO_LISTENER,
	COMPONENT_TYPE_ID_AUDIO_SOURCE,
	COMPONENT_TYPE_ID_PARTICLE_SYSTEM,
	COMPONENT_TYPE_ID_SHADOW_MAP_UPDATE_LISTENER,

	// MUST BE LAST
	NUM_ENGINE_COMPONENT_TYPES,
};

#endif // COMPONENT_TYPE_IDS_H
