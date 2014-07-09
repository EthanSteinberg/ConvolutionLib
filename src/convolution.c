#include "convolution.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <fftw3.h>
#include <string.h>

void slowConvoluteInner(int* indices, int currentDimension, MultidimensionalArray one, MultidimensionalArray two, MultidimensionalArray result);
MultidimensionalArray performConvolutionSlow(MultidimensionalArray one, MultidimensionalArray two);
MultidimensionalArray performConvolutionFast(MultidimensionalArray one, MultidimensionalArray two);

int myPow(int a, int b)
{
    int product = 1;
    for (int i = 0; i < b; ++i)
    {
        product *= a;
    }
    return product;
}

MultidimensionalArray performConvolution(MultidimensionalArray one, MultidimensionalArray two)
{

    if (one.size != two.size || one.dims != two.dims)
        printf("Need to be the same sizes\n");
    MultidimensionalArray result1 =  performConvolutionFast(one,two);
    //MultidimensionalArray result2 = performConvolutionSlow(one,two);
    //printf("I am now done %p %p\n",&result1,&result2);
    //free(result2.arr);
    return result1;
}

MultidimensionalArray createOfSize(MultidimensionalArray source)
{
    MultidimensionalArray result;
    result.arr = malloc(sizeof(double) * myPow(source.size*2,source.dims));
    result.size = source.size*2;
    result.dims = source.dims;
    result.actualSize = myPow(source.size*2,source.dims);

    //printf("Count :%d %d %d\n", myPow(source.size*2,source.dims),source.dims, source.size*2);

    return result;
}


MultidimensionalArray performConvolutionSlow(MultidimensionalArray one, MultidimensionalArray two)
{
    MultidimensionalArray result = createOfSize(one);

    int* indices = malloc(sizeof(int) * result.dims);

    for (int i = 0; i < result.dims; i++)
        indices[i] = 0;

    slowConvoluteInner(indices,0,one,two,result);

    free(indices);

    return result;
}


int getIndex(int *indices, int size, int dims)
{
    int sum = 0;

    for (int i =0; i <dims; i++)
    {
        sum *= size;
        sum += indices[i];
    }
    //printf("%d %d %d\n",indices[0],indices[1],sum);
    return sum;
}

bool isInvalid(int*indices, MultidimensionalArray arr)
{
    for (int i = 0; i < arr.dims; ++i)
    {
        if (indices[i] >= arr.size || indices[i] < 0)
            return true;
    }

    return false;
}

double get(int *indices, MultidimensionalArray arr)
{
    if (isInvalid(indices,arr))
        return 0;
    else
        return arr.arr[getIndex(indices,arr.size,arr.dims)];
}



void set(int *indices, MultidimensionalArray arr, double val)
{
    int location = getIndex(indices,arr.size,arr.dims);
    arr.arr[location] = val;
}

int* getDiff(int *a, int *b, int size)
{
    int* result = malloc(sizeof(int) *size);
    for (int i = 0; i <size; i++)
    {
        result[i] = a[i] - b[i];

    }
    return result;
}

double convoluteInner(int* indices,int* innerIndices,int currentDimension, MultidimensionalArray one, MultidimensionalArray two)
{
    if (currentDimension == one.dims)
    {
        int* otherHalf = getDiff(indices,innerIndices,one.dims);
        double result = get(innerIndices,one) * get(otherHalf,two);
        free(otherHalf);
        return result;
    }
    else
    {
        double sum = 0;
        for (int i = 0; i < one.size; i++) {
            innerIndices[currentDimension] = i;
            sum += convoluteInner(indices,innerIndices,currentDimension+1,one,two);
        }
        return sum;
    }
}

double convolute(int* indices, MultidimensionalArray one, MultidimensionalArray two)
{
    int* innerIndices = malloc(sizeof(int) * one.dims);

    for (int i = 0; i < one.dims; i++)
        innerIndices[i] = 0;

    double result =  convoluteInner(indices,innerIndices,0,one,two);
    free(innerIndices);

    return result;
}



void slowConvoluteInner(int* indices, int currentDimension, MultidimensionalArray one, MultidimensionalArray two, MultidimensionalArray result)
{

    if (currentDimension == result.dims)
        set(indices,result,convolute(indices,one,two));
    else
    {
        for (int i = 0; i < result.size; i++)
        {
            indices[currentDimension] = i;
            slowConvoluteInner(indices,currentDimension+1,one,two,result);
        }
    }

}

ComplexMultidimensionalArray fft(MultidimensionalArray arr)
{

    int* sizes = malloc(sizeof(int) * arr.dims);

    for (int i = 0; i < arr.dims;i++)
        sizes[i] = arr.size;

    int N = myPow(arr.size,arr.dims-1) * (arr.size/2+1);


    ComplexMultidimensionalArray result;
    
    result.arr = fftw_alloc_complex(N);

    result.size = arr.size;
    result.dims = arr.dims;
    result.actualSize = N;

    fftw_plan p = fftw_plan_dft_r2c(arr.dims, sizes,
       arr.arr, result.arr,
       FFTW_ESTIMATE);

    fftw_execute(p); /* repeat as needed */

    fftw_destroy_plan(p);

    free(sizes);

    return result;
}


MultidimensionalArray inverseFFT(ComplexMultidimensionalArray arr)
{
    int* sizes = malloc(sizeof(int) * arr.dims);

    for (int i = 0; i < arr.dims;i++)
        sizes[i] = arr.size;

    int N = myPow(arr.size,arr.dims);

    MultidimensionalArray result;
    
    result.arr = fftw_alloc_real(N);

    result.size = arr.size;
    result.dims = arr.dims;
    result.actualSize = N;

    fftw_plan p = fftw_plan_dft_c2r(arr.dims, sizes,
       arr.arr, result.arr,
       FFTW_ESTIMATE);

    fftw_execute(p); /* repeat as needed */

    fftw_destroy_plan(p);

    free(sizes);

    return result;

}

MultidimensionalArray enlarge(MultidimensionalArray arr)
{
    int N = myPow(arr.size,arr.dims);

    MultidimensionalArray result;
    
    result.arr = fftw_alloc_real(N);

    result.size = arr.size;
    result.dims = arr.dims;
    result.actualSize = N;

    if (result.actualSize != arr.actualSize)
        printf("Bad enlarge, should be same sizes\n");

    memcpy(result.arr,arr.arr,arr.actualSize * sizeof(double));

    return result;
}

MultidimensionalArray performConvolutionFast(MultidimensionalArray one, MultidimensionalArray two)
{   

    one = enlarge(one);
    two = enlarge(two);

    ComplexMultidimensionalArray oneChanged = fft(one);
    ComplexMultidimensionalArray twoChanged = fft(two);

    fftw_free(one.arr);
    fftw_free(two.arr);

    for (int i = 0; i < oneChanged.actualSize; i++)
    {
        oneChanged.arr[i] = oneChanged.arr[i] * twoChanged.arr[i];
        oneChanged.arr[i] /= one.actualSize;
    }

   
    MultidimensionalArray result = inverseFFT(oneChanged);

    fftw_free(oneChanged.arr);
    fftw_free(twoChanged.arr);

    return result;
}
