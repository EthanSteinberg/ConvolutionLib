#ifndef CONVOLUTION_HPP_INCLUDED
#define CONVOLUTION_HPP_INCLUDED

#include <memory>
#include <vector>
#include <complex>

#include <fftw3.h>

class ConvolutionEngine
{
public:
    ConvolutionEngine(int size, int dims);
    ~ConvolutionEngine();
    const double* convolute(const double* a, const double* b);

    const int size;
    const int dims;
    const int totalSize;
    
private:
    const int complexSize;

    struct FFTWDeleter {
        void operator()(void* p) { fftw_free(p); }
    };

    const std::unique_ptr<double[],FFTWDeleter> aCopy;
    const std::unique_ptr<double[],FFTWDeleter> bCopy;

    const std::unique_ptr<std::complex<double>[],FFTWDeleter> aFFT;
    const std::unique_ptr<std::complex<double>[],FFTWDeleter> bFFT;

    const std::unique_ptr<double[],FFTWDeleter> result;

    fftw_plan aFFTPlan;
    fftw_plan bFFTPlan;

    fftw_plan resultInversePlan;
};

#endif //CONVOLUTION_HPP_INCLUDED