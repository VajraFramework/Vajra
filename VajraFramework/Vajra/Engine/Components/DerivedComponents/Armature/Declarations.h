#ifndef ARMATURE_DECLARATIONS_H
#define ARMATURE_DECLARATIONS_H

#define DRAW_BONES 0

// This has to match the declaration of the bone transforms array in shader_variables
#define MAX_BONES 20

#define MAX_BONE_INFUENCES_PER_VERTEX 4

#define USING_BAKED_BONE_MATRICES 1
#if USING_BAKED_BONE_MATRICES
#define USING_RUNTIME_COMPUTED_BONE_MATRICES 0
#else
#define USING_RUNTIME_COMPUTED_BONE_MATRICES 1
#endif

#endif // ARMATURE_DECLARATIONS_H
