#ifdef PLATFORM_ANDROID

#include "ExampleGame/AndroidWrapper/AndroidWrapper.h"
#include "ExampleGame/Test/TestFile.h"

JNIEXPORT void JNICALL Java_com_vajra_examplegame_ExampleGameWrapper_InitGameScene(JNIEnv * env, jobject obj) {
	TestFuntion();
}

#endif // PLATFORM_ANDROID

