#include <stdio.h>
#include "convolution.h"
#include "convolution.hpp"

int main()
{

    Convolution foo(3,2);

    double oneArr[] = {4,3,7,2,7,9,5,2,7}; 

    std::vector<double> oneFoo;
    oneFoo.assign(oneArr,oneArr+9);

    MultidimensionalArray one;
    one.arr = oneArr;
    one.dims = 2;
    one.size=3;
    one.actualSize = 9;

    double twoArr[] = {6,3,7,2,8,4,2,7,5}; 

    std::vector<double> twoFoo;
    twoFoo.assign(twoArr,twoArr+9);

    MultidimensionalArray two;
    two.arr = twoArr;
    two.dims = 2;
    two.size=3;
    two.actualSize = 9;

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

