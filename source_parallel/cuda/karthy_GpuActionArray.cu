#include "karthy_GpuActionArray.h"

karthy::cuda::GpuActionArray::GpuActionArray()
{
	cudaMalloc(&this->d_searchActionSymmetricResult, sizeof(karthy::cuda::GpuAction)); PRINT_ERROR();

	size_t size = BOARD_SIDE_LENGTH * BOARD_SIDE_LENGTH * sizeof(int);
	cudaMalloc(&this->d_elementX, size); PRINT_ERROR();
	cudaMalloc(&this->d_elementY, size); PRINT_ERROR();

	this->elementQValue = new double[BOARD_SIDE_LENGTH * BOARD_SIDE_LENGTH];
	this->nextStateId = new uint64_t[BOARD_SIDE_LENGTH * BOARD_SIDE_LENGTH];
}


void karthy::cuda::GpuActionArray::addAction(const karthy::cuda::GpuAction& actionForAdd)
{
	//cudaMemset(&this->d_elementX[this->elementCount], actionForAdd.x, sizeof(int)); PRINT_ERROR();
	//cudaMemset(&this->d_elementY[this->elementCount], actionForAdd.y, sizeof(int)); PRINT_ERROR();

	cudaMemcpy(&this->d_elementX[this->elementCount], &actionForAdd.x, sizeof(int), cudaMemcpyHostToDevice); PRINT_ERROR();
	cudaMemcpy(&this->d_elementY[this->elementCount], &actionForAdd.y, sizeof(int), cudaMemcpyHostToDevice); PRINT_ERROR();

	this->elementQValue[this->elementCount] = actionForAdd.qValue;
	this->nextStateId[this->elementCount] = actionForAdd.nextStateId;

	if (this->bestAction.isNull() || this->bestAction.qValue < actionForAdd.qValue)
	{
		this->bestAction = actionForAdd;
		this->bestAction.order = elementCount;
	}

	if (this->exploreAction.isNull() && actionForAdd.qValue == 0)
	{
		this->exploreAction = actionForAdd;
		this->exploreAction.order = elementCount;
	}

	this->elementCount++;
}

void karthy::cuda::GpuActionArray::clearAll(void)
{
	this->elementCount = 0;
	this->bestAction.setNull();
	this->exploreAction.setNull();
}
void karthy::cuda::GpuActionArray::print(void)
{
#if DEBUG
	int x;
	int y;

	for (int index = 0; index < this->elementCount; index++)
	{
		cudaMemcpy(&x, &this->d_elementX[index], sizeof(int), cudaMemcpyDeviceToHost); PRINT_ERROR();
		cudaMemcpy(&y, &this->d_elementY[index], sizeof(int), cudaMemcpyDeviceToHost); PRINT_ERROR();
		printf("(%d, %d)\n", x, y);
	}
#endif
}
template <int symmetricType>
__global__ void searchActionSymmetricKernel(
	karthy::cuda::GpuAction* d_searchActionSymmetricResult,
	karthy::cuda::GpuActionArray actionArray,
	karthy::cuda::GpuAction actionForSearch
	)
{
	const int actionIdx = blockIdx.x * blockDim.x + threadIdx.x;

	if (actionIdx >= actionArray.getElementCount()) return;

	//ROTATE 0 FLIP 0
	if (symmetricType == 1)
	{
		if (actionArray.d_elementX[actionIdx] == actionForSearch.x &&
			actionArray.d_elementY[actionIdx] == actionForSearch.y)
		{
			d_searchActionSymmetricResult->x = actionForSearch.x;
			d_searchActionSymmetricResult->y = actionForSearch.y;
			d_searchActionSymmetricResult->order = actionIdx;
			return;
		}
	}

	//ROTATE 90 FLIP 0
	if (symmetricType == 2)
	{
		if (actionArray.d_elementX[actionIdx] == actionForSearch.y &&
			actionArray.d_elementY[actionIdx] == -actionForSearch.x + BOARD_SIDE_LENGTH - 1)
		{
			d_searchActionSymmetricResult->x = actionForSearch.y;
			d_searchActionSymmetricResult->y = -actionForSearch.x + BOARD_SIDE_LENGTH - 1;
			d_searchActionSymmetricResult->order = actionIdx;
			return;
		}
	}

	//ROTATE 180 FLIP 0
	if (symmetricType == 3)
	{
		if (actionArray.d_elementX[actionIdx] == BOARD_SIDE_LENGTH - 1 - actionForSearch.x &&
			actionArray.d_elementY[actionIdx] == BOARD_SIDE_LENGTH - 1 - actionForSearch.y)
		{
			d_searchActionSymmetricResult->x = BOARD_SIDE_LENGTH - 1 - actionForSearch.x;
			d_searchActionSymmetricResult->y = BOARD_SIDE_LENGTH - 1 - actionForSearch.y;
			d_searchActionSymmetricResult->order = actionIdx;
			return;
		}
	}

	//ROTATE 270 FLIP 0
	if (symmetricType == 4)
	{
		if (actionArray.d_elementX[actionIdx] == BOARD_SIDE_LENGTH - 1 - actionForSearch.y &&
			actionArray.d_elementY[actionIdx] == actionForSearch.x)
		{
			d_searchActionSymmetricResult->x = BOARD_SIDE_LENGTH - 1 - actionForSearch.y;
			d_searchActionSymmetricResult->y = actionForSearch.x;
			d_searchActionSymmetricResult->order = actionIdx;
			return;
		}
	}

	//ROTATE 0 FLIP 1
	if (symmetricType == 5)
	{
		if (actionArray.d_elementX[actionIdx] == -actionForSearch.x + BOARD_SIDE_LENGTH - 1 &&
			actionArray.d_elementY[actionIdx] == actionForSearch.y)
		{
			d_searchActionSymmetricResult->x = -actionForSearch.x + BOARD_SIDE_LENGTH - 1;
			d_searchActionSymmetricResult->y = actionForSearch.y;
			d_searchActionSymmetricResult->order = actionIdx;
			return;
		}
	}

	//ROTATE 90 FLIP 1
	if (symmetricType == 6)
	{
		if (actionArray.d_elementX[actionIdx] == actionForSearch.y &&
			actionArray.d_elementY[actionIdx] == actionForSearch.x)
		{
			d_searchActionSymmetricResult->x = actionForSearch.y;
			d_searchActionSymmetricResult->y = actionForSearch.x;
			d_searchActionSymmetricResult->order = actionIdx;
			return;
		}
	}

	//ROTATE 180 FLIP 1
	if (symmetricType == 7)
	{
		if (actionArray.d_elementX[actionIdx] == actionForSearch.x &&
			actionArray.d_elementY[actionIdx] == -actionForSearch.y + BOARD_SIDE_LENGTH - 1)
		{
			d_searchActionSymmetricResult->x = actionForSearch.x;
			d_searchActionSymmetricResult->y = -actionForSearch.y + BOARD_SIDE_LENGTH - 1;
			d_searchActionSymmetricResult->order = actionIdx;
			return;
		}
	}

	//ROTATE 270 FLIP 1
	if (symmetricType == 8)
	{
		if (actionArray.d_elementX[actionIdx] == -actionForSearch.y + BOARD_SIDE_LENGTH - 1 &&
			actionArray.d_elementY[actionIdx] == -actionForSearch.x + BOARD_SIDE_LENGTH - 1)
		{
			d_searchActionSymmetricResult->x = -actionForSearch.y + BOARD_SIDE_LENGTH - 1;
			d_searchActionSymmetricResult->y = -actionForSearch.x + BOARD_SIDE_LENGTH - 1;
			d_searchActionSymmetricResult->order = actionIdx;
			return;
		}
	}
}

karthy::cuda::GpuAction karthy::cuda::GpuActionArray::searchActionSymmetric(const karthy::cuda::GpuAction& actionForSearch, int symmetricType)
{
	karthy::cuda::GpuAction result;

	dim3 dimBlock(BOARD_SIDE_LENGTH);
	dim3 dimGrid(BOARD_SIDE_LENGTH);

	cudaMemset(this->d_searchActionSymmetricResult, 0, sizeof(int));

	switch (symmetricType)
	{
	case(1):
		searchActionSymmetricKernel<1> << <dimGrid, dimBlock >> > (this->d_searchActionSymmetricResult, (*this), actionForSearch);
		break;
	case(2):
		searchActionSymmetricKernel<2> << <dimGrid, dimBlock >> > (this->d_searchActionSymmetricResult, (*this), actionForSearch);
		break;
	case(3):
		searchActionSymmetricKernel<3> << <dimGrid, dimBlock >> > (this->d_searchActionSymmetricResult, (*this), actionForSearch);
		break;
	case(4):
		searchActionSymmetricKernel<4> << <dimGrid, dimBlock >> > (this->d_searchActionSymmetricResult, (*this), actionForSearch);
		break;
	case(5):
		searchActionSymmetricKernel<5> << <dimGrid, dimBlock >> > (this->d_searchActionSymmetricResult, (*this), actionForSearch);
		break;
	case(6):
		searchActionSymmetricKernel<6> << <dimGrid, dimBlock >> > (this->d_searchActionSymmetricResult, (*this), actionForSearch);
		break;
	case(7):
		searchActionSymmetricKernel<7> << <dimGrid, dimBlock >> > (this->d_searchActionSymmetricResult, (*this), actionForSearch);
		break;
	case(8):
		searchActionSymmetricKernel<8> << <dimGrid, dimBlock >> > (this->d_searchActionSymmetricResult, (*this), actionForSearch);
		break;
	}

	cudaMemcpy(&result, this->d_searchActionSymmetricResult, sizeof(karthy::cuda::GpuAction), cudaMemcpyDeviceToHost);
	result.nextStateId = this->nextStateId[result.order];

	return result;
}

karthy::cuda::GpuAction karthy::cuda::GpuActionArray::getBestAction(void)
{
	return this->bestAction;
}

karthy::cuda::GpuAction karthy::cuda::GpuActionArray::getExploreAction(void)
{
	return this->exploreAction;
}

karthy::cuda::GpuAction karthy::cuda::GpuActionArray::getActionByIndex(int index)
{
	karthy::cuda::GpuAction result;

	cudaMemcpy(&result.x, &this->d_elementX[index], sizeof(int), cudaMemcpyDeviceToHost); PRINT_ERROR();
	cudaMemcpy(&result.y, &this->d_elementY[index], sizeof(int), cudaMemcpyDeviceToHost); PRINT_ERROR();

	//printf("%d %d\n", index, this->elementCount);
	//printf("%f\n", this->elementQValue[index]);

	result.qValue = this->elementQValue[index];
	result.nextStateId = this->nextStateId[index];
	result.order = index;

	return result;
}

int karthy::cuda::GpuActionArray::getElementCount(void)
{
	return elementCount;
}
