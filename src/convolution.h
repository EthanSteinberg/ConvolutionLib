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


MultidimensionalArray createOfSize(MultidimensionalArray source);


MultidimensionalArray performConvolution(MultidimensionalArray one, MultidimensionalArray two);

MultidimensionalArray enlarge(MultidimensionalArray arr);



#endif
