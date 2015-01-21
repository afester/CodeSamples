#include <Objbase.h>

#include <stdio.h>

#include "com_example_java2com_COMProxy.h"
#include "COMProxy.h"


static COMProxy* comProxy = 0;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
	comProxy = new COMProxy(jvm);

	return JNI_VERSION_1_2;
}


// Note: There is no guarantee that OnUnload is ever called!
// TODO: How can we properly call ::CoUninitialize??
/*JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *jvm, void *reserved) {
 fprintf(stderr, "*** OnUnload ...\n");
 if (comProxy) {
 delete comProxy;
 }
 comProxy = 0;
 }
 */


JNIEXPORT void JNICALL Java_com_example_java2com_COMProxy_Create
(JNIEnv* env, jobject dispatchObj, jstring className) {
	if (comProxy) {
		comProxy->create(env, dispatchObj, className);
	}
}

JNIEXPORT void JNICALL Java_com_example_java2com_COMProxy_Get
  (JNIEnv *env, jobject dispatchObj, jstring className) {
	if (comProxy) {
		comProxy->get(env, dispatchObj, className);
	}
}

JNIEXPORT jobject JNICALL Java_com_example_java2com_COMProxy_Invoke(
		JNIEnv *env, jobject dispatchObj, jstring memberName, jint wFlags,
		jobjectArray parameters) {
	if (comProxy) {
		return comProxy->invoke(env, dispatchObj, memberName, wFlags,
				parameters);
	}

	return 0;
}


JNIEXPORT void JNICALL Java_com_example_java2com_COMProxy_Destroy
(JNIEnv* env, jobject dispatchObj) {
	if (comProxy) {
		comProxy->release(env, dispatchObj);
	}
}

JNIEXPORT void JNICALL Java_com_example_java2com_COMProxy_SetDebugEnabled
  (JNIEnv *env, jclass clazz, jboolean flag) {
	if (comProxy) {
		comProxy->setDebugEnabled(env, flag);
	}
}
