

#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#if 0
typedef struct {
	char *name;       
	char *signature;
	void *fnPtr;
	
}JNINativeMethod;

#endif 

jintArray  c_hello(JNIEnv *env, jobject cls, jintArray arr) {

	jint *carr;
	jint *oarr;
	jintArray rarr;
	
	jint i, n = 0;
	carr = (*env)->GetIntArrayElements(env, arr, NULL);
	if (carr == NULL) {
		return 0; /* exception occurred */
	}

    n = (*env)->GetArrayLength(env, arr);
	oarr = malloc(sizeof(jint)*n);
	if(oarr == NULL){
		(*env)->ReleaseIntArrayElements(env, arr, carr, 0);
		return 0;
	}
	
	for(i = 0; i < n; i++){
		oarr[i] = carr[n-1-i];
	}

	/* create jintArray */
	rarr = (*env)->NewIntArray(env, n);
	if(rarr == NULL){
		return 0;
	}

	(*env)->SetIntArrayRegion(env, rarr, 0, n, oarr);
	free(oarr);
	
	return rarr;

}

static const JNINativeMethod methods[] = {
 {"hello", "([I)[I", (void*)c_hello};
};




JNIEXPORT jint JNICALL

JNI_OnLoad(JavaVM *jvm, void *reserved)
{
	JNIEnv *env;
	jclass cls;
	
	if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_4)) {
		return JNI_ERR; /* JNI version not supported */
	}
	cls = (*env)->FindClass(env, "JNIDemo");
	if (cls == NULL) {
		return JNI_ERR;
	}
	/* Use weak global ref to allow C class to be unloaded */
	Class_C = (*env)->NewWeakGlobalRef(env, cls);
	if (Class_C == NULL) {
		return JNI_ERR;
	}
	/* Compute and cache the method ID */
	MID_C_g = (*env)->GetMethodID(env, cls, "g", "()V");
	if (MID_C_g == NULL) {
		return JNI_ERR;
	}


	if((*env)->RegisterNatives(env, cls, &methods, 1) < 0){
		retrun JNI_ERR;
	}
	
		return JNI_VERSION_1_4;
}


