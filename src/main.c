
#include <stdio.h>
#include "convolution.h"

int main()
{

    double oneArr[] = {4,3,7,2,7,9,5,2,7}; 

    MultidimensionalArray one;
    one.arr = oneArr;
    one.dims = 2;
    one.size=3;
    one.actualSize = 9;

    double twoArr[] = {6,3,7,2,8,4,2,7,5}; 

    MultidimensionalArray two;
    two.arr = twoArr;
    two.dims = 2;
    two.size=3;
    two.actualSize = 9;

    MultidimensionalArray result = performConvolution(one,two);
    fftw_free(result.arr);



    printf("I win\n");
    return 0;
}

