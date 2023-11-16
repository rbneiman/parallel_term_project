#include "bitonicsort.cuh"
#include "cuda.h"


#define getLastCudaError(msg)      getLastCudaError (msg, __FILE__, __LINE__)

inline void getLastCudaError_(const char *errorMessage, const char *file, const int line)
{
    cudaError_t err = cudaGetLastError();

    if (cudaSuccess != err)
    {
        fprintf(stderr, "%s(%i) : getLastCudaError() CUDA error : %s : (%d) %s.\n",
                file, line, errorMessage, (int)err, cudaGetErrorString(err));
        fflush(stderr);
        //DEVICE_RESET
        //        exit(EXIT_FAILURE);
    }
}




namespace gpu_sort{

    void bitonicSort(std::vector<double>& arr){

    }
}