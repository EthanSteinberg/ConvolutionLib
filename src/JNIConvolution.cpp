#include "convolutionlib_JNIConvolution.h"
#include "convolution.h"
#include <stdio.h>
#include <math.h>

MultidimensionalArray enclose(JNIEnv *env, jdoubleArray arr, jint size, jint dims)
{
    MultidimensionalArray aEnclosed;
    aEnclosed.arr = env->GetDoubleArrayElements(arr,NULL);
    aEnclosed.size = size;
    aEnclosed.dims = dims;
    aEnclosed.actualSize = (int) pow(size,dims);

    return aEnclosed;

}

jdoubleArray extract(JNIEnv *env, MultidimensionalArray arr)
{
    jdoubleArray result = env->NewDoubleArray(arr.actualSize);

    env->SetDoubleArrayRegion(result,0,arr.actualSize,arr.arr);

    return result;
}

JNIEXPORT jdoubleArray JNICALL Java_convolutionlib_JNIConvolution_performConvolutionInC
  (JNIEnv *env, jclass myself, jdoubleArray a, jdoubleArray b, jint size, jint dims)
{
    MultidimensionalArray aEnclosed = enclose(env,a,size,dims);
    MultidimensionalArray bEnclosed = enclose(env,b,size,dims);

    MultidimensionalArray result = performConvolution(aEnclosed,bEnclosed);

    env->ReleaseDoubleArrayElements(a, aEnclosed.arr,JNI_ABORT);

    env->ReleaseDoubleArrayElements(b, bEnclosed.arr,JNI_ABORT);


    jdoubleArray javaResult = extract(env,result);

    fftw_free(result.arr);

    return javaResult;
}

