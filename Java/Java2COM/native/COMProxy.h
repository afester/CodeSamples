#include <jni.h>

class COMProxy {

public:
	COMProxy(JavaVM *jvm);

	~COMProxy();

/* Public API - bound to the IDispatch class in Java */
	void create(JNIEnv* env, jobject dispatchObj, jstring className);

	void get(JNIEnv* env, jobject dispatchObj, jstring className);

	jobject invoke(JNIEnv *env, jobject dispatchObj, jstring memberName,
				   jint wFlags, jobjectArray parameters);

	void release(JNIEnv* env, jobject dispatchObj);

	void setDebugEnabled(JNIEnv* env, jboolean flag);

private:
	void log(const char* format, ...);
	void dumpVariantRec(const VARIANT* var, int level);
	void dumpVariant(const VARIANT* var);
	void dumpParams(const DISPPARAMS* dsp);

	void convertToVariant(const VARIANT* value, jobject resultVariant);

	// From JAVA Variant to C++ VARIANT
	VARIANT createVariant(jobject variant);

	// From C++ VARIANT to JAVA Variant
	jobject createVariant(const VARIANT* value);

	void createParams(DISPPARAMS* params, int wFlags, jobjectArray parameters);

	void updateParams(DISPPARAMS* params, int wFlags, jobjectArray parameters);

	void freeParams(DISPPARAMS* params);

	JNIEnv* env;
	jboolean debugEnabled;

	// Field- and Method IDs from IDispatch
	jmethodID idispatchConstructorMID;
	jfieldID dispatchHandleFID;

	// Field- and Method IDs from Variant
	jmethodID variantConstructorMID;
	jfieldID vtFID;
	jfieldID intValueFID;
	jfieldID floatValueFID;
	jfieldID doubleValueFID;
	jfieldID booleanValueFID;
	jfieldID strValueFID;
	jfieldID dispatchValueFID;

};
