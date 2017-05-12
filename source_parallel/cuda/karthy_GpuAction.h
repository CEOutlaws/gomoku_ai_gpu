#pragma once
#include "karthy_GpuCommon.h"

namespace karthy
{
	namespace cuda
	{
		class GpuAction
		{
		public:
			int x;
			int y;
			double qValue;
			uint64_t nextStateId;
			int order;

			CUDA_CALLABLE_MEMBER GpuAction() {}
			CUDA_CALLABLE_MEMBER GpuAction(int x, int y);

			void operator = (const GpuAction &rhsAction);
			void setNull(void);
			bool isNull(void);
		};
	}
}

