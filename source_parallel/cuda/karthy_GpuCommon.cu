#include "karthy_GpuCommon.h"

void karthy::cuda::printError(void)
{
#if DEBUG
	cudaDeviceSynchronize();
	cudaError_t error = cudaGetLastError();
	if (error != cudaSuccess)
	{
		// print the CUDA error message and exit
		printf("CUDA error: %s\n", cudaGetErrorString(error));
	}
#endif
}