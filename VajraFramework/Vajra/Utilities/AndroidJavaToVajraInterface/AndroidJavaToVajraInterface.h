#ifndef ANDROID_JAVA_TO_VAJRA_INTERFACE_H
#define ANDROID_JAVA_TO_VAJRA_INTERFACE_H

#ifdef PLATFORM_ANDROID

#include <jni.h>
#include <android/log.h>

extern "C" {
    JNIEXPORT void JNICALL Java_com_vajra_androidwrapper_VajraAndroidWrapper_touchEventOccurred(JNIEnv * env, jobject obj, jint id, jfloat x, jfloat y, jint touchEventType);
};

#endif // PLATFORM_ANDROID

#endif // ANDROID_JAVA_TO_VAJRA_INTERFACE_H
