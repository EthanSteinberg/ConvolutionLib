#include "convolution.h"
#include "convolution.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <fftw3.h>
#include <string.h>

static int myPow(int a, int b)
{
    int product = 1;
    for (int i = 0; i < b; ++i)
    {
        product *= a;
    }
    return product;
}

std::complex<double>* allocateComplexFFTW(int complexSize)
{
     return reinterpret_cast<std::complex<double>*>(fftw_alloc_complex(complexSize));
}


ConvolutionEngine::ConvolutionEngine(int size, int dims) : 
    size(size), 
    dims(dims), 
    totalSize(myPow(size,dims)),
    complexSize(myPow(size,dims-1) * (size/2+1)),

    aCopy(fftw_alloc_real(totalSize)),
    bCopy(fftw_alloc_real(totalSize)),

    aFFT(allocateComplexFFTW(complexSize)),
    bFFT(allocateComplexFFTW(complexSize)),

    result(fftw_alloc_real(totalSize))
{
    std::vector<int> sizes(dims,size);


    aFFTPlan = fftw_plan_dft_r2c(dims, sizes.data(),
       aCopy.get(), reinterpret_cast< double (*) [2]>(aFFT.get()),
       FFTW_PATIENT);

    bFFTPlan = fftw_plan_dft_r2c(dims, sizes.data(),
       bCopy.get(), reinterpret_cast< double (*) [2]>(bFFT.get()),
       FFTW_PATIENT);

    resultInversePlan = fftw_plan_dft_c2r(dims, sizes.data(),
       reinterpret_cast< double (*) [2]>(aFFT.get()), result.get(),
       FFTW_PATIENT);
}

ConvolutionEngine::~ConvolutionEngine()
{
    fftw_destroy_plan(aFFTPlan);
    fftw_destroy_plan(bFFTPlan);
    fftw_destroy_plan(resultInversePlan);
}

const double* ConvolutionEngine::convolute(const double* a, const double* b)
{
    memcpy(aCopy.get(),a,totalSize*sizeof(double));
    memcpy(bCopy.get(),b,totalSize*sizeof(double));

    fftw_execute(aFFTPlan);
    fftw_execute(bFFTPlan);

    for (int i = 0; i < complexSize; i++)
    {
        aFFT.get()[i] *= bFFT.get()[i]/static_cast<double>(totalSize);
    }

    fftw_execute(resultInversePlan);

    return result.get();
}

MultidimensionalArray performConvolutionFast(MultidimensionalArray one, MultidimensionalArray two);



MultidimensionalArray performConvolution(MultidimensionalArray one, MultidimensionalArray two)
{

    if (one.size != two.size || one.dims != two.dims)
        printf("Need to be the same sizes\n");
    MultidimensionalArray result1 =  performConvolutionFast(one,two);
    return result1;
}


ComplexMultidimensionalArray fft(MultidimensionalArray arr)
{

    int* sizes = new int[arr.dims];

    for (int i = 0; i < arr.dims;i++)
        sizes[i] = arr.size;

    int N = myPow(arr.size,arr.dims-1) * (arr.size/2+1);


    ComplexMultidimensionalArray result;
    
    result.arr = reinterpret_cast<std::complex<double>*>(fftw_alloc_complex(N));

    result.size = arr.size;
    result.dims = arr.dims;
    result.actualSize = N;

    fftw_plan p = fftw_plan_dft_r2c(arr.dims, sizes,
       arr.arr, reinterpret_cast< double (*) [2]>(result.arr),
       FFTW_ESTIMATE);

    fftw_execute(p); /* repeat as needed */

    fftw_destroy_plan(p);

    free(sizes);

    return result;
}


MultidimensionalArray inverseFFT(ComplexMultidimensionalArray arr)
{
    int* sizes = new int[arr.dims];

    for (int i = 0; i < arr.dims;i++)
        sizes[i] = arr.size;

    int N = myPow(arr.size,arr.dims);

    MultidimensionalArray result;
    
    result.arr = fftw_alloc_real(N);

    result.size = arr.size;
    result.dims = arr.dims;
    result.actualSize = N;

    fftw_plan p = fftw_plan_dft_c2r(arr.dims, sizes,
       reinterpret_cast< double (*) [2]>(arr.arr), result.arr,
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
        oneChanged.arr[i] *= twoChanged.arr[i]/static_cast<double>(one.actualSize);
    }

   
    MultidimensionalArray result = inverseFFT(oneChanged);

    fftw_free(oneChanged.arr);
    fftw_free(twoChanged.arr);

    return result;
}
