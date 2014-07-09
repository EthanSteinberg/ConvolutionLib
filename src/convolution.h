#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <complex.h> 

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
    complex *arr;
    int dims;
    int size;
    int actualSize;
} ComplexMultidimensionalArray;


MultidimensionalArray performConvolution(MultidimensionalArray one, MultidimensionalArray two);


#endif
