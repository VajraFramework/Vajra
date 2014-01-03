#ifndef VAJRA_TO_ANDROID_JAVA_INTERFACE_H
#define VAJRA_TO_ANDROID_JAVA_INTERFACE_H

#ifdef PLATFORM_ANDROID

#include <jni.h>
#include <android/log.h>

#include <string>


class VajraToAndroidJavaInterface {
public:
	// TODO [Implement] Implement thread safety here
	static inline JNIEnv* GetJNIEnv() { return VajraToAndroidJavaInterface::jniEnv; }
	static void SetJNIEnv(JNIEnv* jniEnv_) { VajraToAndroidJavaInterface::jniEnv = jniEnv_; }

private:
	static JNIEnv* jniEnv;
};

std::string GetAndroidDeviceBaseResourcesPath();

#endif // PLATFORM_ANDROID

#endif // VAJRA_TO_ANDROID_JAVA_INTERFACE_H
