#include "karthy_GpuMatrix.h"

karthy::cuda::GpuMatrix::GpuMatrix()
{
	cudaMalloc(&this->d_element, BOARD_SIDE_LENGTH * BOARD_SIDE_LENGTH * sizeof(int)); PRINT_ERROR();
}

void karthy::cuda::GpuMatrix::setElement(const karthy::cuda::GpuAction& action, int taker)
{
	setElement(action.y, action.x, taker);
}

void karthy::cuda::GpuMatrix::setElement(int row, int col, int value)
{
	cudaMemcpy(&this->d_element[row * BOARD_SIDE_LENGTH + col], &value, sizeof(int), cudaMemcpyHostToDevice); PRINT_ERROR();
}

__global__ void setAllElementKernel(int* d_element, int value)
{
	d_element[blockIdx.x * blockDim.x + threadIdx.x] = value;
}

void karthy::cuda::GpuMatrix::setAllElement(int value)
{
	dim3 dimBlock(BOARD_SIDE_LENGTH);
	dim3 dimGrid(BOARD_SIDE_LENGTH);

	setAllElementKernel << <dimGrid, dimBlock >> > (this->d_element, value); PRINT_ERROR();
}

void karthy::cuda::GpuMatrix::print(void)
{
#if DEBUG
	for (int boxIndexY = 0; boxIndexY < BOARD_SIDE_LENGTH; boxIndexY++)
	{
		for (int boxIndexX = 0; boxIndexX < BOARD_SIDE_LENGTH; boxIndexX++)
		{
			int value;
			cudaMemcpy(&value, &this->d_element[boxIndexY * BOARD_SIDE_LENGTH + boxIndexX], sizeof(int), cudaMemcpyDeviceToHost); PRINT_ERROR();
			printf("%d", value);
		}
		printf("\n");
	}
#endif
}
