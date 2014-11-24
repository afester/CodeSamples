#include <Objbase.h>
#include <stdio.h>

#include "COMProxy.h"

COMProxy::COMProxy(JavaVM *jvm) : debugEnabled(false) {
	// initialize the COM library
	fprintf(stderr, "*** Initializing COM API ...\n");
	::CoInitialize( NULL);

	// cache field IDs

	JNIEnv* env = 0;
	jvm->AttachCurrentThread((void **)&env, NULL);

	// NOTE: FindClass returns local reference only! Only method- and Field IDs
	// are valid forever, class references are NOT!!!!
	jclass idispatchClass = env->FindClass("com/example/java2com/IDispatch");
	idispatchConstructorMID = env->GetMethodID(idispatchClass, "<init>", "()V");
	dispatchHandleFID = env->GetFieldID(idispatchClass, "dispatchHandle", "I");

	jclass variantClass = env->FindClass("com/example/java2com/Variant");
	variantConstructorMID = env->GetMethodID(variantClass, "<init>", "()V");
	vtFID = env->GetFieldID(variantClass, "vt", "I");
	intValueFID = env->GetFieldID(variantClass, "intValue", "I");
	booleanValueFID = env->GetFieldID(variantClass, "booleanValue", "Z");
	strValueFID = env->GetFieldID(variantClass, "strValue", "Ljava/lang/String;");
	dispatchValueFID = env->GetFieldID(variantClass, "dispatch", "Lcom/example/java2com/IDispatch;");
}


COMProxy::~COMProxy() {
	// uninitialize the COM library
	fprintf(stderr, "*** Uninitializing the COM API ...\n");
	::CoUninitialize();
}


void COMProxy::create(JNIEnv* env, jobject dispatchObj, jstring className) {
	if (debugEnabled)
		fprintf(stderr, "*** Create() ...\n");

	// Get the class name from the String parameter
	OLECHAR * szClassname = (OLECHAR*) env->GetStringChars(className, NULL);

	// get the class id for the required object
	if (debugEnabled)
		fprintf(stderr, "  Retrieving Class ID for \"%ls\" ...\n", szClassname);
	CLSID CLSID_Class;
	HRESULT hr = ::CLSIDFromProgID(szClassname, &CLSID_Class);
	if (debugEnabled) {
		fprintf(stderr, "    Status: 0x%lx\n", hr);
		fprintf(stderr, "    Result: 0x%08lX-%04hX-%04hX-%08lX\n",
				CLSID_Class.Data1, CLSID_Class.Data2, CLSID_Class.Data3,
				(long) CLSID_Class.Data4);
	}

	// Create an instance of the requested class, and query the required interfaces
	fprintf(stderr, "  Retrieving IDispatch interface ...\n");
	IDispatch* pServer = 0;
	hr = ::CoCreateInstance(CLSID_Class, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch,
							reinterpret_cast<void**> (&pServer));
	if (debugEnabled) {
		fprintf(stderr, "    Status: 0x%lx\n", hr);
		fprintf(stderr, "    Result: 0x%p\n", pServer);
	}

	// store the pointer into the dispatchObj
	// set the dispatchHandle member - NOTE: architecture specific! (32 bit <=> 64 bit)
	env->SetIntField(dispatchObj, dispatchHandleFID, (int) pServer);

	// Clean up
	env->ReleaseStringChars(className, (jchar*) szClassname);
}


jobject COMProxy::invoke(JNIEnv *env, jobject dispatchObj, jstring memberName,
						 jint wFlags, jobjectArray parameters) {
	if (debugEnabled) {
		fprintf(stderr, "*** Invoke() ...\n");
	}

	// Get the member name from the String parameter
	OLECHAR * szMember = (OLECHAR*) env->GetStringChars(memberName, NULL);

	// get the IDispatch pointer from the dispatchObj
	IDispatch* pServer = (IDispatch*) env->GetIntField(dispatchObj, dispatchHandleFID);

	// Retrieve the dispatch ID for the member
	DISPID dispId = 0;
	if (debugEnabled) {
		fprintf(stderr, "  Retrieving dispatch id for \"%ls\" ...\n", szMember);
	}

	HRESULT hr = pServer->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_USER_DEFAULT, &dispId);
    switch(hr) {
		case DISP_E_UNKNOWNNAME :
			fprintf(stderr, "    ERROR: Unknown member\n");
			break;

		default :
			if (debugEnabled) {
				fprintf(stderr, "    Error code: 0x%lx\n", hr);
			}
			break;
	}
	if (debugEnabled) {
		fprintf(stderr, "    Result: 0x%lx\n", dispId);	// NOTE: 0x0 is the default method!
													// (e.g. "Item" for collections)
	}

	// Convert the Parameters object into a COM DISPPARAMS structure
	DISPPARAMS params;
	createParams(env, &params, wFlags, parameters);

	// Invoke the member (property GET, property SET or Invoke)
	if (debugEnabled) {
		fprintf(stderr, "  Invoking member ... (%d)\n", (int) wFlags);
	}
	VARIANT result;
	hr = pServer->Invoke(dispId, IID_NULL, 0, wFlags, &params, &result, NULL, NULL);
	switch(hr) {
		case DISP_E_PARAMNOTOPTIONAL :
			fprintf(stderr, "    ERROR: Missing non-optional parameter\n");
			break;

		case DISP_E_TYPEMISMATCH :
			fprintf(stderr, "    ERROR: Parameter type mismatch\n");
			break;

		case DISP_E_MEMBERNOTFOUND :
			fprintf(stderr, "    ERROR: Member not found\n");
			break;

		default :
			if (debugEnabled) {
				fprintf(stderr, "    Error code: 0x%lx\n", hr);
			}
			break;
	}

	// Convert the result back to a Variant object
	jobject resultVariant = convertToVariant(env, &result);

	// cleanup
	freeParams(&params);
	env->ReleaseStringChars(memberName, (jchar*) szMember);

	return resultVariant;
}


void COMProxy::release(JNIEnv* env, jobject dispatchObj) {
	if (debugEnabled) {
		fprintf(stderr, "*** Release()...\n");
	}

	// get the IDispatch pointer from the dispatchObj
	IDispatch* pServer = (IDispatch*) env->GetIntField(dispatchObj, dispatchHandleFID);

	// Release the pointer to the instance
	int newCount = pServer->Release();
	if (debugEnabled) {
		fprintf(stderr, "    New refcount: %d\n", newCount); // (should be 0)
	}
}


void COMProxy::setDebugEnabled(JNIEnv* env, jboolean flag) {
	debugEnabled = flag;
}


jobject COMProxy::convertToVariant(JNIEnv *env, const VARIANT* value) {

	// Create a new instance of com.example.java2com.Variant
	if (debugEnabled) {
		fprintf(stderr, "  FindClass(com/example/java2com/Variant)\n");
	}
	jclass variantClass = env->FindClass("com/example/java2com/Variant");
	if (debugEnabled) {
		fprintf(stderr, "  newObject()\n");
	}
	jobject resultVariant = env->NewObject(variantClass, variantConstructorMID);

	// Set the vt field of the Variant instance
	// jfieldID fid = env->GetFieldID(variantClass, "vt", "I");
	env->SetIntField(resultVariant, vtFID, value->vt);

	// depending on the Variant type, set the corresponding field in the Variant instance
	switch (value->vt) {
		case VT_BSTR:
			{
				if (debugEnabled) {
					fprintf(stderr, "  Result: \"%ls\"", value->bstrVal);
				}

				// Convert the BSTRING into an UTF8 string

				// get the strValue field of the Variant instance -
				// TODO: too much magic here - are all assumptions correct??
				// See also http://stackoverflow.com/questions/606075/how-to-convert-char-to-bstr
				int stringLength = *((int*) (value->bstrVal) - 1) / 2;
				if (debugEnabled) {
					fprintf(stderr, " (length: %d)\n", stringLength);
				}

				jstring resString = env->NewString((jchar*) value->bstrVal,
												   stringLength);
				env->SetObjectField(resultVariant, strValueFID, resString);
			}
			break;

		case VT_DISPATCH:
			{
				if (debugEnabled) {
					fprintf(stderr, "  Result: 0x%p\n", value->pdispVal);
				}

				// Create a new instance of com.example.java2com.IDispatch
				if (debugEnabled) {
					fprintf(stderr, "  FindClass(com/example/java2com/IDispatch)\n");
				}
				jclass idispatchClass = env->FindClass("com/example/java2com/IDispatch");
				if (debugEnabled) {
					fprintf(stderr, "  newObject()\n");
				}
				jobject idispatchObject = env->NewObject(idispatchClass, idispatchConstructorMID);

				// set the dispatchHandle member - NOTE: architecture specific! (32 bit <=> 64 bit)
				env->SetIntField(idispatchObject, dispatchHandleFID, (int) value->pdispVal);

				// set the dispatch member to the created object
				env->SetObjectField(resultVariant, dispatchValueFID, idispatchObject);
			}
			break;

		case VT_I4:
			{
				if (debugEnabled) {
					fprintf(stderr, "  Result: %d\n", value->iVal);
				}
				env->SetIntField(resultVariant, intValueFID, value->iVal);
			}
			break;

		case VT_BOOL:
			{
				if (debugEnabled) {
					fprintf(stderr, "  Result: %d\n", value->boolVal);
				}
				env->SetBooleanField(resultVariant, booleanValueFID, value->boolVal ? true : false);
			}
			break;

		default:
			// UnsupportedOperationException TODO
			fprintf(stderr, "  UNSUPPORTED DATA TYPE: %d\n", value->vt);
			break;
	}

	return resultVariant;
}

// TODO
static DISPID mydispid = DISPID_PROPERTYPUT;

void COMProxy::createParams(JNIEnv* env, DISPPARAMS* params, int wFlags,
							jobjectArray parameters) {
	if (debugEnabled) {
		fprintf(stderr, "  Converting Variant[] to DISPPARAMS ...\n");
	}

	// initialize DISPPARAMS structure
	params->rgvarg = NULL;
	params->rgdispidNamedArgs = NULL;
	params->cArgs = 0;
	params->cNamedArgs = 0;

	// convert Variant array into VARIANT structure
	if (parameters != 0) {
		if (debugEnabled) {
			fprintf(stderr, "    Retrieving length of Variant[] array ...\n");
		}

		int size = env->GetArrayLength(parameters);
		if (debugEnabled) {
			fprintf(stderr, "    Number of parameters: %d\n", size);
		}
		if (size > 0) {
			params->rgvarg = (VARIANTARG*) calloc(size, sizeof(VARIANTARG));
			if (debugEnabled) {
				fprintf(stderr, "      Allocated memory at: 0x%p\n", params->rgvarg);
			}

			for (int idx = 0; idx < size; idx++) {
				params->rgvarg[idx].vt = VT_EMPTY;

				jobject variantElement = env->GetObjectArrayElement(parameters, idx);

				int vt = env->GetIntField(variantElement, vtFID);
				switch (vt) {
					case VT_BSTR:
						{
							if (debugEnabled) {
								fprintf(stderr, "Converting java string to BSTR ...");
							}

							jstring valStr = (jstring) env->GetObjectField(variantElement, strValueFID);
							const jchar* val = env->GetStringChars(valStr, NULL);
							int valLen = env->GetStringLength(valStr);

							// TODO: Need to deallocate string in freeParams()
							BSTR bstr = SysAllocStringLen((OLECHAR*) val, valLen);

							if (debugEnabled) {
								char *x = (char*) bstr;
								fprintf(stderr, " %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X \n", x[-4], x[-3], x[-2], x[-1],
												  x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]);
							}

							params->rgvarg[idx].vt = VT_BSTR;
							params->rgvarg[idx].bstrVal = bstr;
						}
						break;

					case VT_BOOL:
						{
							bool val = env->GetBooleanField(variantElement, booleanValueFID);
							params->rgvarg[idx].vt = VT_BOOL;
							params->rgvarg[idx].boolVal = val;
						}
						break;

					case VT_I4:
						{
							int val = env->GetIntField(variantElement, intValueFID);
							params->rgvarg[idx].vt = VT_I4;
							params->rgvarg[idx].iVal = val;
						}
						break;
				}
			}
			params->cArgs = size;

			// Create named parameters

			// TODO: need to further investigate this.
			if (wFlags == DISPATCH_PROPERTYPUT) {
				params->rgdispidNamedArgs = &mydispid;
				params->cNamedArgs = 1;
			}
		}
	}
}


void COMProxy::freeParams(DISPPARAMS* params) {
	if (params->cArgs > 0) {
		if (debugEnabled) {
			fprintf(stderr, "      Freeing memory at: 0x%p\n", params->rgvarg);
		}

		free(params->rgvarg);
		params->rgvarg = 0;
		params->cArgs = 0;
	}
}
