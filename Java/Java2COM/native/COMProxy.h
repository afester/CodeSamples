#include <jni.h>

class COMProxy {

public:
	COMProxy(JavaVM *jvm);

	~COMProxy();

	void create(JNIEnv* env, jobject dispatchObj, jstring className);

	jobject invoke(JNIEnv *env, jobject dispatchObj, jstring memberName,
				   jint wFlags, jobjectArray parameters);

	void release(JNIEnv* env, jobject dispatchObj);

private:
	jobject convertToVariant(JNIEnv *env, const VARIANT* value);

	void createParams(JNIEnv* env, DISPPARAMS* params, int wFlags,
					  jobjectArray parameters);

	void freeParams(DISPPARAMS* params);


	// Field- and Method IDs from IDispatch
	jmethodID idispatchConstructorMID;
	jfieldID dispatchHandleFID;

	// Field- and Method IDs from Variant
	jmethodID variantConstructorMID;
	jfieldID vtFID;
	jfieldID intValueFID;
	jfieldID booleanValueFID;
	jfieldID strValueFID;
	jfieldID dispatchValueFID;

};
