/**
 */

#include <iostream>

#include "com_example_Native.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
	std::cerr << "JNI_OnLoad()" << std::endl;

	return JNI_VERSION_1_2;
}


JNIEXPORT void JNICALL Java_com_example_Native_helloWorld(JNIEnv *env, jobject self) {
	std::cerr << "Hello World of JNI" << std::endl;
}


JNIEXPORT jint JNICALL Java_com_example_Native_sumArray(JNIEnv *env, jobject self, jintArray arr) {
	std::cerr << "Summing up all array elements ..." << std::endl;

	jint len = env->GetArrayLength(arr);
	jint *body = env->GetIntArrayElements(arr, 0);

	jint result = 0;
	for (int idx = 0;  idx < len;  idx++) {
		result = result + body[idx];
	}

	env->ReleaseIntArrayElements(arr, body, 0);
	return result;
}
