#include <Objbase.h>
#include <stdio.h>

#include "COMProxy.h"


COMProxy::COMProxy(JavaVM *jvm) : debugEnabled(false) {
	// initialize the COM library
	log("*** Initializing COM API ...\n");
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
	floatValueFID = env->GetFieldID(variantClass, "floatValue", "F");
	doubleValueFID = env->GetFieldID(variantClass, "doubleValue", "D");
	booleanValueFID = env->GetFieldID(variantClass, "booleanValue", "Z");
	strValueFID = env->GetFieldID(variantClass, "strValue", "Ljava/lang/String;");
	dispatchValueFID = env->GetFieldID(variantClass, "dispatch", "Lcom/example/java2com/IDispatch;");
}


COMProxy::~COMProxy() {
	// uninitialize the COM library
	log("*** Uninitializing the COM API ...\n");
	::CoUninitialize();
}


void COMProxy::log(const char* format, ...) {
	if (debugEnabled) {
		va_list argptr;
		va_start(argptr, format);
		vfprintf(stderr, format, argptr);
		va_end(argptr);
	}
}


void COMProxy::create(JNIEnv* theEnv, jobject dispatchObj, jstring className) {
	this->env = theEnv;
	log("*** Create() ...\n");

	// Get the class name from the String parameter
	OLECHAR * szClassname = (OLECHAR*) env->GetStringChars(className, NULL);

	// get the class id for the required object
	log("  Retrieving Class ID for \"%ls\" ...\n", szClassname);
	CLSID CLSID_Class;
	HRESULT hr = ::CLSIDFromProgID(szClassname, &CLSID_Class);
	log("    Status: 0x%lx\n", hr);
	log("    Result: 0x%08lX-%04hX-%04hX-%08lX\n",
				CLSID_Class.Data1, CLSID_Class.Data2, CLSID_Class.Data3,
				(long) CLSID_Class.Data4);

	// Create an instance of the requested class, and query the required interfaces
	log("  Creating COM object ...\n");
	IDispatch* pServer = 0;
	hr = ::CoCreateInstance(CLSID_Class, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch,
							reinterpret_cast<void**> (&pServer));
	log("    Status: 0x%lx\n", hr);
	log("    Result: 0x%p\n", pServer);

/*
	switch(hr) {
	case S_OK : break;
	case REGDB_E_CLASSNOTREG : break;
	case LASS_E_NOAGGREGATION : break;
	case E_NOINTERFACE : break;
	case E_POINTER : break;
	}
*/

	// store the pointer into the dispatchObj
	// set the dispatchHandle member - NOTE: architecture specific! (32 bit <=> 64 bit)
	env->SetIntField(dispatchObj, dispatchHandleFID, (int) pServer);

	// Clean up
	env->ReleaseStringChars(className, (jchar*) szClassname);
}


void COMProxy::get(JNIEnv* theEnv, jobject dispatchObj, jstring className) {
	this->env = theEnv;

	// Get the class name from the String parameter
	OLECHAR * szClassname = (OLECHAR*) env->GetStringChars(className, NULL);

	// get the class id for the required object
	log("  Retrieving Class ID for \"%ls\" ...\n", szClassname);
	CLSID CLSID_Class;
	HRESULT hr = ::CLSIDFromProgID(szClassname, &CLSID_Class);
	log("    Status: 0x%lx\n", hr);
	log("    Result: 0x%08lX-%04hX-%04hX-%08lX\n",
				CLSID_Class.Data1, CLSID_Class.Data2, CLSID_Class.Data3,
				(long) CLSID_Class.Data4);

	IDispatch* pServer = 0;

	log("\n::GetActiveObject()"); // \"%ls\", NULL, IID_IDispatch, &pServer)\n", szClassname);
	hr = ::GetActiveObject(CLSID_Class, NULL, reinterpret_cast<IUnknown**> (&pServer));
	switch(hr) {
		case S_OK :
			break;

		case E_FAIL :
		default:
			fprintf(stderr, "    ERROR: Unspecified error: 0x%lx\n", hr);
			break;
	}

	log("    Result: 0x%p\n", pServer);

#if 0
	log("\n::CoGetObject(\"%ls\", NULL, IID_IDispatch, &pServer)\n", szClassname);
	HRESULT hr = ::CoGetObject(szClassname, NULL, IID_IDispatch, reinterpret_cast<void**> (&pServer));

	switch(hr) {
		case S_OK :
			break;

		case MK_E_SYNTAX :
			fprintf(stderr, "    ERROR: The pszName parameter is not a properly formed display name.\n");
			break;

		case MK_E_NOOBJECT :
			fprintf(stderr, "    ERROR: The object identified by this moniker, or some object identified by the composite moniker of which this moniker is a part, could not be found.\n");
			break;

		case MK_E_EXCEEDEDDEADLINE :
			fprintf(stderr, "    ERROR: The binding operation could not be completed within the time limit specified by the BIND_OPTS structure passed in pBindOptions.\n");
			break;

		case MK_E_CONNECTMANUALLY :
			fprintf(stderr, "    ERROR: The binding operation requires assistance from the end user. The most common reasons for returning this value are that a password is needed or that a floppy needs to be mounted.\n");
			break;

		case MK_E_INTERMEDIATEINTERFACENOTSUPPORTED:
			fprintf(stderr, "    ERROR: An intermediate object was found but it did not support an interface required to complete the binding operation. For example, an item moniker returns this value if its container does not support the IOleItemContainer interface.\n");
			break;

		case E_FAIL :
		default:
			fprintf(stderr, "    ERROR: Unspecified error: 0x%lx\n", hr);
			break;
	}

	log("    Result: 0x%p\n", pServer);

ITypeInfo* info;
hr = pServer->GetTypeInfo(0, 0, &info);
log("    Result2: 0x%p\n", hr);
TYPEATTR* typeAttr;
hr = info->GetTypeAttr(&typeAttr);
log("    Result3: 0x%p\n", hr);
log("    cFuncs: %d\n"
    "    cVars: %d\n"
	"    cImplTypes: %d\n", typeAttr->cFuncs,typeAttr->cVars,typeAttr->cImplTypes);
#endif

	// store the pointer into the dispatchObj
	// set the dispatchHandle member - NOTE: architecture specific! (32 bit <=> 64 bit)
	env->SetIntField(dispatchObj, dispatchHandleFID, (int) pServer);

	// Clean up
	env->ReleaseStringChars(className, (jchar*) szClassname);
}


void COMProxy::dumpVariantRec(const VARIANT* var, int level) {
	if (var->vt & VT_BYREF) {
		switch(var->vt & 0xfff) {
			case VT_I2:
			case VT_I4:
				log("        %d 0x%04x 0x%08x -> %f\n", level, var->vt, var->pintVal, *var->pintVal);
				break;

			case VT_R4:
				log("        %d 0x%04x 0x%08x -> %f\n", level, var->vt, var->pfltVal, *var->pfltVal);
				break;

			case VT_R8:
				log("        %d 0x%04x 0x%08x -> %f\n", level, var->vt, var->pdblVal, *var->pdblVal);
				break;

			default :
				log("        %d 0x%04x 0x%08x\n", level, var->vt, var->punkVal);
				break;
		}

	} else {
		switch(var->vt & 0xfff) {
			case VT_VARIANT:
				log("        %d 0x%04x %08p\n", level, var->vt, var->pvarVal);
				dumpVariantRec(var->pvarVal, level +1);
				break;

			case VT_I2:
			case VT_I4:
				log("        %d 0x%04x %d\n", level, var->vt, var->iVal);
				break;

			case VT_R4:
				log("        %d 0x%04x %f\n", level, var->vt, var->fltVal);
				break;

			case VT_R8:
				log("        %d 0x%04x %f\n", level, var->vt, var->dblVal);
				break;

			default :
				log("        %d 0x%04x %f \n", level, var->vt, var->dblVal);
				break;
		}
	}
}


void COMProxy::dumpVariant(const VARIANT* var) {
	dumpVariantRec(var, 0);
}

void COMProxy::dumpParams(const DISPPARAMS* dsp) {
	log("      dsp.cArgs: %d\n", dsp->cArgs);
	for (int i = 0;  i < dsp->cArgs;  i++) {
		VARIANT* var = &dsp->rgvarg[i];
		dumpVariant(var);
	}

	log("      dsp.cNamedArgs: %d\n", dsp->cNamedArgs);

}




jobject COMProxy::invoke(JNIEnv *theEnv, jobject dispatchObj, jstring memberName,
						 jint wFlags, jobjectArray parameters) {
	this->env = theEnv;
	log("\n*** Invoke() ...\n");

	// Get the member name from the String parameter
	OLECHAR * szMember = (OLECHAR*) env->GetStringChars(memberName, NULL);

	// get the IDispatch pointer from the dispatchObj
	IDispatch* pServer = (IDispatch*) env->GetIntField(dispatchObj, dispatchHandleFID);

	// Retrieve the dispatch ID for the member
	DISPID dispId = 0;
	log("  Retrieving dispatch id for \"%ls\" ...\n", szMember);

	HRESULT hr = pServer->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_USER_DEFAULT, &dispId);
    switch(hr) {
    	case ERROR_SUCCESS :
    		break;

    	case ERROR_INVALID_HANDLE :
			fprintf(stderr, "    ERROR: Invalid handle\n");
			break;

		case DISP_E_UNKNOWNNAME :
			fprintf(stderr, "    ERROR: Unknown member\n");
			break;

		default :
			fprintf(stderr, "    Error code: 0x%lx\n", hr);
			break;
	}
	log("    Result: 0x%lx\n\n", dispId);	// NOTE: 0x0 is the default method!
										// (e.g. "Item" for collections)

	// Convert the Parameters object into a COM DISPPARAMS structure
	DISPPARAMS params;
	createParams(&params, wFlags, parameters);

	log("    PARAMETERS BEFORE INVOKE:\n");
	dumpParams(&params);

	// Invoke the member (property GET, property SET or Invoke)
	log("  Invoking member ... (%d)\n", (int) wFlags);

	VARIANT result;
	UINT uArgErr;
	hr = pServer->Invoke(dispId, IID_NULL, 0, wFlags, &params, &result, NULL, &uArgErr);
	switch(hr) {
		case ERROR_SUCCESS:
			break;


    	case ERROR_ACCESS_DENIED :
			fprintf(stderr, "    ERROR: Access denied\n");
    		break;

		case DISP_E_PARAMNOTOPTIONAL :
			fprintf(stderr, "    ERROR: Missing non-optional parameter\n");
			break;

		case DISP_E_TYPEMISMATCH :
			fprintf(stderr, "    ERROR: Parameter type mismatch: %d\n", uArgErr);
			break;

		case 0x800706f4 :
			fprintf(stderr, "    ERROR: A null reference pointer was passed to the stub: %d\n", uArgErr);
			break;

		case DISP_E_MEMBERNOTFOUND :
			fprintf(stderr, "    ERROR: Member not found\n");
			break;

		case DISP_E_BADPARAMCOUNT :
			fprintf(stderr, "    ERROR: Invalid number of parameters\n");
			break;

		case DISP_E_PARAMNOTFOUND  :
		case DISP_E_UNKNOWNNAME  :
		case DISP_E_NONAMEDARGS  :
		case DISP_E_BADVARTYPE  :
		case DISP_E_EXCEPTION  :
		case DISP_E_OVERFLOW  :
		case DISP_E_BADINDEX  :
		case DISP_E_UNKNOWNLCID  :
		case DISP_E_ARRAYISLOCKED  :
		case DISP_E_BADCALLEE  :
		case DISP_E_NOTACOLLECTION  :
		case DISP_E_DIVBYZERO  :

		default :
			fprintf(stderr, "    Error code: 0x%lx\n", hr);
			break;
	}

	log("    PARAMETERS AFTER INVOKE:");
	dumpParams(&params);
	log("    RESULT AFTER INVOKE:");
	dumpVariant(&result);

	// Convert the result back to a Variant object
	jobject resultVariant = createVariant(&result);

	// Update the Parameter array for any [out] parameter
	updateParams(&params, wFlags, parameters);

	// cleanup
	freeParams(&params);
	env->ReleaseStringChars(memberName, (jchar*) szMember);

	log("\n");

	return resultVariant;
}


void COMProxy::release(JNIEnv* theEnv, jobject dispatchObj) {
	this->env = theEnv;

	// get the IDispatch pointer from the dispatchObj
	IDispatch* pServer = (IDispatch*) env->GetIntField(dispatchObj, dispatchHandleFID);

	// Release the pointer to the instance
	log("\n::Release() 0x%08x\n", pServer);
	int newCount = pServer->Release();

	log("    New refcount: %d\n", newCount); // (should be 0)
}


void COMProxy::setDebugEnabled(JNIEnv* theEnv, jboolean flag) {
	this->env = theEnv;
	debugEnabled = flag;
}


void COMProxy::convertToVariant(const VARIANT* value, jobject resultVariant) {


	// Set the vt field of the Variant instance
	// jfieldID fid = env->GetFieldID(variantClass, "vt", "I");
	env->SetIntField(resultVariant, vtFID, value->vt);

	// depending on the Variant type, set the corresponding field in the Variant instance
	switch (value->vt & 0xfff) {
		case VT_BSTR:
			{
				log("  Result: \"%ls\"", value->bstrVal);

				// Convert the BSTRING into an UTF8 string

				// get the strValue field of the Variant instance -
				// TODO: too much magic here - are all assumptions correct??
				// See also http://stackoverflow.com/questions/606075/how-to-convert-char-to-bstr
				int stringLength = *((int*) (value->bstrVal) - 1) / 2;
				log(" (length: %d)\n", stringLength);

				jstring resString = env->NewString((jchar*) value->bstrVal,
												   stringLength);
				env->SetObjectField(resultVariant, strValueFID, resString);
			}
			break;

		case VT_DISPATCH:
			{
				log("  Result: 0x%p\n", value->pdispVal);

				// Create a new instance of com.example.java2com.IDispatch
				log("  FindClass(com/example/java2com/IDispatch)\n");
				jclass idispatchClass = env->FindClass("com/example/java2com/IDispatch");

				log("  newObject()\n");
				jobject idispatchObject = env->NewObject(idispatchClass, idispatchConstructorMID);

				// set the dispatchHandle member - NOTE: architecture specific! (32 bit <=> 64 bit)
				env->SetIntField(idispatchObject, dispatchHandleFID, (int) value->pdispVal);

				// set the dispatch member to the created object
				env->SetObjectField(resultVariant, dispatchValueFID, idispatchObject);
			}
			break;

		case VT_I2:
		case VT_I4:
			{
				int result = 0;
				if (value->vt & VT_BYREF) {
					result = *value->piVal;
				} else {
					result = value->iVal;
				}

				log("  Result: %d\n", result);
				env->SetIntField(resultVariant, intValueFID, result);
			}
			break;

		case VT_BOOL:
			{
				log("  Result: %d\n", value->boolVal);
				env->SetBooleanField(resultVariant, booleanValueFID, value->boolVal ? true : false);
			}
			break;

		case VT_R4 :
			{
				float result = 0;
				if (value->vt & VT_BYREF) {
					result = *value->pfltVal;
				} else {
					result = value->fltVal;
				}

				log("  Result: %f\n", result);
				env->SetFloatField(resultVariant, floatValueFID, result);
			}
			break;

		case VT_R8 :
			{
				double result = 0;
				if (value->vt & VT_BYREF) {
					result = *value->pdblVal;
				} else {
					result = value->dblVal;
				}

				log("  Result: %f\n", result);
				env->SetDoubleField(resultVariant, doubleValueFID, result);
			}
			break;

		default:
			// UnsupportedOperationException TODO
			log("  UNSUPPORTED DATA TYPE: %d\n", value->vt);
			break;
	}
}


jobject COMProxy::createVariant(const VARIANT* value) {
	// Create a new instance of com.example.java2com.Variant
	log("  FindClass(com/example/java2com/Variant)\n");
	jclass variantClass = env->FindClass("com/example/java2com/Variant");

	log("  newObject()\n");
	jobject resultVariant = env->NewObject(variantClass, variantConstructorMID);

	convertToVariant(value, resultVariant);

	return resultVariant;
}

// TODO
static DISPID mydispid = DISPID_PROPERTYPUT;

VARIANT COMProxy::createVariant(jobject variant) {
	// get the type identifier
	int vt = env->GetIntField(variant, vtFID);

	// create a result structure
	VARIANT result;
	memset(&result, 0, sizeof(VARIANT));
	VariantInit(&result);
	result.vt = VT_EMPTY;

	switch (vt & 0x0FFF) {	// upper bits contain flags
		case VT_BSTR:
			{
				log("Converting java string to BSTR ...");

				jstring valStr = (jstring) env->GetObjectField(variant, strValueFID);
				const jchar* val = env->GetStringChars(valStr, NULL);
				int valLen = env->GetStringLength(valStr);

				// TODO: Need to deallocate string in freeParams()
				BSTR bstr = SysAllocStringLen((OLECHAR*) val, valLen);

				char *x = (char*) bstr;
				log(" %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X \n",
					x[-4], x[-3], x[-2], x[-1],
					x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]);

				result.vt = vt;
				result.bstrVal = bstr;
			}
			break;

		case VT_BOOL:
			{
				bool val = env->GetBooleanField(variant, booleanValueFID);
				result.vt = vt;
				result.boolVal = val;
			}
			break;

		case VT_I4:
			{
				int val = env->GetIntField(variant, intValueFID);
				result.vt = vt;
				result.iVal = val;
			}
			break;

		case VT_R4 :
			{
				float val = env->GetFloatField(variant, floatValueFID);
				result.vt = vt;
				result.fltVal = val;
			}
			break;

		case VT_R8 :
			{
				double val = env->GetDoubleField(variant, doubleValueFID);
				result.vt = vt;
				result.dblVal = val;
			}
			break;

		default:
			log("UNSUPPORTED VARIANT TYPE: %d\n", vt);
	}

	// wrap value in a reference variant
	if (vt & VT_BYREF) {
		//VARIANT* val = (VARIANT*) calloc(1, sizeof(VARIANT));
		//VariantInit(val);
		//result.vt = VT_EMPTY;
		//*val = result;

		double* refVal = (double*) calloc(1, sizeof(double));
		result.pdblVal = refVal;
	}

	return result;
}


void COMProxy::createParams(DISPPARAMS* params, int wFlags,
							jobjectArray parameters) {
	log("  Converting Variant[] to DISPPARAMS ...\n");

	// initialize DISPPARAMS structure
	params->rgvarg = NULL;
	params->rgdispidNamedArgs = NULL;
	params->cArgs = 0;
	params->cNamedArgs = 0;

	// convert Variant array into VARIANT structure
	if (parameters != 0) {
		log("    Retrieving length of Variant[] array ...\n");

		int size = env->GetArrayLength(parameters);
		log("    Number of parameters: %d\n", size);
		if (size > 0) {
			params->rgvarg = (VARIANTARG*) calloc(size, sizeof(VARIANTARG));
			log("      Allocated memory at: 0x%p\n", params->rgvarg);

			// Note: DISPPARAMS entries are in reverse order!
			int res = 0;
			for (int idx = size - 1; idx >= 0; idx--) {
				// get next Variant object from the parameter array
				jobject variantElement = env->GetObjectArrayElement(parameters, idx);
				params->rgvarg[res++] = createVariant(variantElement);
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


void COMProxy::updateParams(DISPPARAMS* params, int wFlags,
							jobjectArray parameters) {
	log("  Updating Variant[] out parameters from DISPPARAMS ...\n");

	if (parameters != 0) {
		log("    Retrieving length of Variant[] array ...\n");

		int size = env->GetArrayLength(parameters);
		log("    Number of parameters: %d\n", size);
		if (size > 0) {

			int res = 0;
			for (int idx = size - 1; idx >= 0; idx--) {
				// get next Variant object from the parameter array
				jobject variant = env->GetObjectArrayElement(parameters, idx);
				int vt = env->GetIntField(variant, vtFID);
				if (vt & VT_BYREF) {
					log("    UPDATING #%d\n", idx);

					// write the VARIANT value into the JAVA Variant object
					convertToVariant(&params->rgvarg[res], variant);
				}
				res++;
			}
		}
	}
}


void COMProxy::freeParams(DISPPARAMS* params) {
	if (params->cArgs > 0) {
		log("      Freeing memory at: 0x%p\n", params->rgvarg);

		// TODO: iterate array elements and free any memory for reference values!
		// ...

		// free the VARIANTARGS array
		free(params->rgvarg);
		params->rgvarg = 0;
		params->cArgs = 0;
	}
}
