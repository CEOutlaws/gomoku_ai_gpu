#include "karthy_AiGpu.h"

karthy::cuda::AiGpu::AiGpu()
{
	cudaMalloc(&this->d_getSymmetricTypeResult, sizeof(int));  PRINT_ERROR();
}

__global__ void getSymmetricTypeKernel(
	int* result,
	karthy::cuda::GpuMatrix matrix1, 
	karthy::cuda::GpuMatrix matrix2,
	karthy::cuda::GpuAction actionForCheck,
	karthy::cuda::GpuActionArray actionArray)
{
	//int row = threadIdx.y; 
	//int col = threadIdx.x;
	__shared__ int s_mat1[BOARD_SIDE_LENGTH][BOARD_SIDE_LENGTH];
	__shared__ int s_mat2[BOARD_SIDE_LENGTH][BOARD_SIDE_LENGTH];
	__shared__ int s_equalMatrixResult;
	__shared__ int s_equalPointResult;
	__shared__ karthy::cuda::GpuAction s_arrayAction;

	s_mat1[threadIdx.y][threadIdx.x] = matrix1.d_element[threadIdx.y * blockDim.x + threadIdx.x];
	s_mat2[threadIdx.y][threadIdx.x] = matrix2.d_element[threadIdx.y * blockDim.x + threadIdx.x];
	/*
#if DEBUG
	if (blockIdx.x == 0 && threadIdx.x == 0 && threadIdx.y == 0)
	{
		printf("smat1\n");

		for (int boxIndexY = 0; boxIndexY < BOARD_SIDE_LENGTH; boxIndexY++)
		{
			for (int boxIndexX = 0; boxIndexX < BOARD_SIDE_LENGTH; boxIndexX++)
			{
				printf("%d", s_mat1[boxIndexY][boxIndexX]);
			}
			printf("\n");
		}
		printf("smat2\n");
		for (int boxIndexY = 0; boxIndexY < BOARD_SIDE_LENGTH; boxIndexY++)
		{
			for (int boxIndexX = 0; boxIndexX < BOARD_SIDE_LENGTH; boxIndexX++)
			{
				printf("%d", s_mat2[boxIndexY][boxIndexX]);
			}
			printf("\n");
		}
	}
#endif
*/

	//mat2 vs mat1 ROTATE_000_FLIP_0
	if (threadIdx.x == 0 && threadIdx.y == 0)
	{
		s_arrayAction.x = actionArray.d_elementX[blockIdx.x];
		s_arrayAction.y = actionArray.d_elementY[blockIdx.x];

		s_mat1[s_arrayAction.y][s_arrayAction.x] = actionArray.taker;
		s_mat2[actionForCheck.y][actionForCheck.x] = actionArray.taker;

		s_equalMatrixResult = 1;
		s_equalPointResult =
			s_arrayAction.x == actionForCheck.x &&
			s_arrayAction.y == actionForCheck.y;
	}

	__syncthreads();

	if (s_equalPointResult)
	{
		if (s_mat1[threadIdx.y][threadIdx.x] != s_mat2[threadIdx.y][threadIdx.x])
		{
			s_equalMatrixResult = 0;
		}

		__syncthreads();

		if (s_equalMatrixResult)
		{
			if (threadIdx.x == 0 && threadIdx.y == 0)
			{
				*result = 1;
			}
			return;
		}
	}

	//mat2 vs mat1 ROTATE_090_FLIP_0
	if (threadIdx.x == 0 && threadIdx.y == 0)
	{
		s_equalMatrixResult = 1;
		s_equalPointResult =
			s_arrayAction.x == actionForCheck.y &&
			s_arrayAction.y == -actionForCheck.x + BOARD_SIDE_LENGTH - 1;
	}

	__syncthreads();

	if (s_equalPointResult)
	{
		if (s_mat1[threadIdx.y][threadIdx.x] != s_mat2[threadIdx.x][BOARD_SIDE_LENGTH - 1 - threadIdx.y])
		{
			s_equalMatrixResult = 0;
		}
		__syncthreads();

		if (s_equalMatrixResult)
		{
			if (threadIdx.x == 0 && threadIdx.y == 0)
			{
				*result = 2;
			}
			return;
		}
	}

	//mat2 vs mat1 ROTATE_180_FLIP_0
	if (threadIdx.x == 0 && threadIdx.y == 0)
	{
		s_equalMatrixResult = 1;
		s_equalPointResult =
			s_arrayAction.x == BOARD_SIDE_LENGTH - 1 - actionForCheck.x &&
			s_arrayAction.y == BOARD_SIDE_LENGTH - 1 - actionForCheck.y;
	}

	__syncthreads();

	if (s_equalPointResult)
	{
		if (s_mat1[threadIdx.y][threadIdx.x] != s_mat2[BOARD_SIDE_LENGTH - 1 - threadIdx.y][BOARD_SIDE_LENGTH - 1 - threadIdx.x])
		{
			s_equalMatrixResult = 0;
		}
		__syncthreads();

		if (s_equalMatrixResult)
		{
			if (threadIdx.x == 0 && threadIdx.y == 0)
			{
				*result = 3;
			}
			return;
		}
	}

	//mat2 vs mat1 ROTATE_270_FLIP_0
	if (threadIdx.x == 0 && threadIdx.y == 0)
	{
		s_equalMatrixResult = 1;
		s_equalPointResult =
			s_arrayAction.x == BOARD_SIDE_LENGTH - 1 - actionForCheck.y &&
			s_arrayAction.y == actionForCheck.x;
	}

	__syncthreads();

	if (s_equalPointResult)
	{
		if (s_mat1[threadIdx.y][threadIdx.x] != s_mat2[-threadIdx.x + BOARD_SIDE_LENGTH - 1][threadIdx.y])
		{
			s_equalMatrixResult = 0;
		}
		__syncthreads();

		if (s_equalMatrixResult)
		{
			if (threadIdx.x == 0 && threadIdx.y == 0)
			{
				*result = 4;
			}
			return;
		}
	}

	//mat2 vs mat1 0 ROTATE_000_FLIP_1

	if (threadIdx.x == 0 && threadIdx.y == 0)
	{
		s_equalMatrixResult = 1;
		s_equalPointResult =
			s_arrayAction.x == -actionForCheck.x + BOARD_SIDE_LENGTH - 1 &&
			s_arrayAction.y == actionForCheck.y;
	}

	__syncthreads();

	if (s_equalPointResult)
	{
		if (s_mat1[threadIdx.y][threadIdx.x] != s_mat2[threadIdx.y][-threadIdx.x + BOARD_SIDE_LENGTH - 1])
		{
			s_equalMatrixResult = 0;
		}
		__syncthreads();

		if (s_equalMatrixResult)
		{
			if (threadIdx.x == 0 && threadIdx.y == 0)
			{
				*result = 5;
			}
			return;
		}
	}
	//mat2 vs mat1 ROTATE_090_FLIP_1
	if (threadIdx.x == 0 && threadIdx.y == 0)
	{
		s_equalMatrixResult = 1;
		s_equalPointResult =
			s_arrayAction.x == actionForCheck.y &&
			s_arrayAction.y == actionForCheck.x;
	}

	__syncthreads();

	if (s_equalPointResult)
	{
		if (s_mat1[threadIdx.y][threadIdx.x] != s_mat2[threadIdx.x][threadIdx.y])
		{
			s_equalMatrixResult = 0;
		}
		__syncthreads();

		if (s_equalMatrixResult)
		{
			if (threadIdx.x == 0 && threadIdx.y == 0)
			{
				*result = 6;
			}
			return;
		}
	}

	//mat2 vs mat1 ROTATE_180_FLIP_1
	if (threadIdx.x == 0 && threadIdx.y == 0)
	{
		s_equalMatrixResult = 1;
		s_equalPointResult =
			s_arrayAction.x == actionForCheck.x &&
			s_arrayAction.y == -actionForCheck.y + BOARD_SIDE_LENGTH - 1;
	}

	__syncthreads();

	if (s_equalPointResult)
	{
		if (s_mat1[threadIdx.y][threadIdx.x] != s_mat2[-threadIdx.y + BOARD_SIDE_LENGTH - 1][threadIdx.x])
		{
			s_equalMatrixResult = 0;
		}
		__syncthreads();

		if (s_equalMatrixResult)
		{
			if (threadIdx.x == 0 && threadIdx.y == 0)
			{
				*result = 7;
			}
			return;
		}
	}

	//mat2 vs mat1 ROTATE_270_FLIP_1
	if (threadIdx.x == 0 && threadIdx.y == 0)
	{
		s_equalMatrixResult = 1;
		s_equalPointResult = 
			s_arrayAction.x == -actionForCheck.y + BOARD_SIDE_LENGTH - 1 &&
			s_arrayAction.y == -actionForCheck.x + BOARD_SIDE_LENGTH - 1;
	}

	__syncthreads();

	if (s_equalPointResult)
	{
		if (s_mat1[threadIdx.y][threadIdx.x] != s_mat2[-threadIdx.x + BOARD_SIDE_LENGTH - 1][-threadIdx.y + BOARD_SIDE_LENGTH - 1])
		{
			s_equalMatrixResult = 0;
		}
		__syncthreads();

		if (s_equalMatrixResult)
		{
			if (threadIdx.x == 0 && threadIdx.y == 0)
			{
				*result = 8;
			}
			return;
		}
	}
}

int karthy::cuda::AiGpu::getSymmetricType(const GpuAction& action, const GpuMatrix& onActionMatrix)
{
	if (this->actionArray.getElementCount() == 0)
	{
		return 0;
	}

	int result;

	dim3 dimBlock(BOARD_SIDE_LENGTH, BOARD_SIDE_LENGTH);
	dim3 dimGrid(this->actionArray.getElementCount());

	cudaMemset(this->d_getSymmetricTypeResult, 0, sizeof(int)); PRINT_ERROR();

	getSymmetricTypeKernel << <dimGrid, dimBlock >> > (this->d_getSymmetricTypeResult, this->logicalMatrix, onActionMatrix,
		action, this->actionArray); PRINT_ERROR();

	cudaMemcpy(&result, d_getSymmetricTypeResult, sizeof(int), cudaMemcpyDeviceToHost); PRINT_ERROR();

	return result;
}

void karthy::cuda::AiGpu::reset(void)
{
	this->actionArray.clearAll();
	this->logicalMatrix.setAllElement(2);
	this->physicalMatrix.setAllElement(2);
}
