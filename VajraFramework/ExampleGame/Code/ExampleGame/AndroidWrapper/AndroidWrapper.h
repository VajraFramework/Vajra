#ifdef PLATFORM_ANDROID

#ifndef ANDROID_WRAPPER_H
#define ANDROID_WRAPPER_H

#include <jni.h>
#include <android/log.h>

extern "C" {
    JNIEXPORT void JNICALL Java_com_vajra_examplegame_ExampleGameWrapper_InitGameScene(JNIEnv * env, jobject obj);
};


#endif // ANDROID_WRAPPER_H

#endif // PLATFORM_ANDROID

