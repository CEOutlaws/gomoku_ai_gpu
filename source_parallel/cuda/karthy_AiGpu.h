#pragma once
#include "karthy_GpuMatrix.h"
#include "karthy_GpuActionArray.h"
#include "karthy_GpuAction.h"
#include "karthy_GpuCommon.h"

namespace karthy
{
	namespace cuda
	{
		class AiGpu
		{
		private:
			int* d_getSymmetricTypeResult;		

		public:
			GpuMatrix logicalMatrix;
			GpuMatrix physicalMatrix;
			GpuActionArray actionArray;

			AiGpu();

			//matrix: the matrix on which action is willing to perform
			int getSymmetricType(const GpuAction& action, const GpuMatrix& onActionMatrix);
			void reset(void);
		};
	}
}