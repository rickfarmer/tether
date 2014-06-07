#include "proxy.h"
#include <jni.h>
// Note jni.h is only required for android NDK 

jstring
Java_com_rickfarmer_tether_Tether_proxy(JNIEnv* env, jobject thiz, jint start)
{
	int res = proxy(start);
	
    char *szFormat = "Proxy state: %i";  
    char *szResult;
    
	// malloc room for the resulting string  
    szResult = malloc(sizeof(szFormat) + 20);  
    
	// standard sprintf  
    sprintf(szResult, szFormat, res);  
	
    // get an object string  
    jstring result = (*env)->NewStringUTF(env, szResult);  
    
	// cleanup  
    free(szResult);  
    
	return result;
}