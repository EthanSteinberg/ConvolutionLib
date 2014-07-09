#include "convolution.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <fftw3.h>
#include <string.h>

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
    return result1;
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
