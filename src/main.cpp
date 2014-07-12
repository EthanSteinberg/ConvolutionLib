#include <stdio.h>
#include "convolution.h"
#include "convolution.hpp"

int main()
{

    ConvolutionEngine foo(6,2);

    double oneArr[] = { 4,3,7, 0,0,0, 2,7,9, 0,0,0, 5,2,7, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
    }; 


    double twoArr[] = { 6,3,7, 0,0,0, 2,8,4, 0,0,0, 2,7,5, 0,0,0,
                        0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,

    };

   
    MultidimensionalArray one;
    one.arr = oneArr;
    one.dims = 2;
    one.size=6;
    one.actualSize = 36;

    MultidimensionalArray two;
    two.arr = twoArr;
    two.dims = 2;
    two.size=6;
    two.actualSize = 36;

    MultidimensionalArray result = performConvolution(one,two);
    
    for (int i = 0; i < result.actualSize; i++)
        printf("%lf ",result.arr[i]);

    printf("\n");

    fftw_free(result.arr);


    const double* secondResult = foo.convolute(oneArr,twoArr);

    for (int i = 0; i < result.actualSize; i++)
        printf("%lf ",secondResult[i]);

    printf("\n");


    printf("I win\n");
    return 0;
}

