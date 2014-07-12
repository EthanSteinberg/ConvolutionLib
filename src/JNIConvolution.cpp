#include "convolutionlib_JNIConvolution.h"
#include "convolution.h"
#include "convolution.hpp"
#include <stdio.h>



static int myPow(int a, int b)
{
    int product = 1;
    for (int i = 0; i < b; ++i)
    {
        product *= a;
    }
    return product;
}

MultidimensionalArray enclose(JNIEnv *env, jdoubleArray arr, jint size, jint dims)
{
    MultidimensionalArray aEnclosed;
    aEnclosed.arr = env->GetDoubleArrayElements(arr,NULL);
    aEnclosed.size = size;
    aEnclosed.dims = dims;
    aEnclosed.actualSize = myPow(size,dims);

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


JNIEXPORT jlong JNICALL Java_convolutionlib_JNIConvolution_setupConvolutionEngine
  (JNIEnv *env, jclass myself, jint size, jint dims)
{
    ConvolutionEngine* engine = new ConvolutionEngine(size,dims);
    jlong result = reinterpret_cast<jlong>(engine);
    return result;
}

JNIEXPORT void JNICALL Java_convolutionlib_JNIConvolution_releaseConvolutionEngine
  (JNIEnv *env, jclass myself, jlong enginePointer)
{
    ConvolutionEngine* engine = reinterpret_cast<ConvolutionEngine*>(enginePointer);
    delete engine;
}

JNIEXPORT jdoubleArray JNICALL Java_convolutionlib_JNIConvolution_performConvolution
  (JNIEnv *env, jclass myself, jlong enginePointer, jdoubleArray a, jdoubleArray b)
{
    ConvolutionEngine* engine = reinterpret_cast<ConvolutionEngine*>(enginePointer);

    double* aArr = reinterpret_cast<double*>(env->GetPrimitiveArrayCritical(a,NULL));
    double* bArr = reinterpret_cast<double*>(env->GetPrimitiveArrayCritical(b,NULL));
    
    const double* result = engine->convolute(aArr,bArr);

    env->ReleasePrimitiveArrayCritical(a, aArr,JNI_ABORT);
    env->ReleasePrimitiveArrayCritical(b, bArr,JNI_ABORT);

    jdoubleArray javaResult = env->NewDoubleArray(engine->totalSize);
    env->SetDoubleArrayRegion(javaResult,0,engine->totalSize,result);

    return javaResult;
}

