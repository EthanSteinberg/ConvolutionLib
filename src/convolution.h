#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <complex>

#include <fftw3.h>

typedef struct
{
    double *arr;
    int dims;
    int size;
    int actualSize;
} MultidimensionalArray;

typedef struct
{
    std::complex<double> *arr;
    int dims;
    int size;
    int actualSize;
} ComplexMultidimensionalArray;


MultidimensionalArray performConvolution(MultidimensionalArray one, MultidimensionalArray two);


#endif
