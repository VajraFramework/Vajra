#include "Vajra/Engine/Input/Platforms/AndroidInputSender.h"
#include "Vajra/Utilities/AndroidJavaToVajraInterface/AndroidJavaToVajraInterface.h"

#ifdef PLATFORM_ANDROID

JNIEXPORT void JNICALL Java_com_vajra_androidwrapper_VajraAndroidWrapper_touchEventOccurred(JNIEnv * env, jobject obj, jint id, jfloat x, jfloat y, jint touchEventType) {
	AndroidInputSender::touchEventOccurred(id, x, y, touchEventType);
}

#endif // PLATFORM_ANDROID
