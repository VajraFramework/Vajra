#ifdef PLATFORM_ANDROID

#include "Vajra/Utilities/VajraToAndroidJavaInterface/VajraToAndroidJavaInterface.h"

JNIEnv* VajraToAndroidJavaInterface::jniEnv = nullptr;

std::string GetAndroidDeviceBaseResourcesPath() {
    jclass clazz = VajraToAndroidJavaInterface::GetJNIEnv()->FindClass("com/vajra/androidwrapper/VajraAndroidWrapper");
    jmethodID methodID = VajraToAndroidJavaInterface::GetJNIEnv()->GetStaticMethodID(clazz, "GetDeviceResourcesBasePath", "()Ljava/lang/String;");
    jobject resultJString = VajraToAndroidJavaInterface::GetJNIEnv()->CallStaticObjectMethod(clazz, methodID);
    const char* cString = VajraToAndroidJavaInterface::GetJNIEnv()->GetStringUTFChars((jstring)resultJString, NULL);
    std::string returnString = cString;
    VajraToAndroidJavaInterface::GetJNIEnv()->ReleaseStringUTFChars((jstring)resultJString, cString);
    return returnString;
}

#endif // PLATFORM_ANDROID
