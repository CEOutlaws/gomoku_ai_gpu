#include "../karthy_ShareCommon.h"
#include "karthy_GpuAction.h"

namespace karthy
{
	namespace cuda
	{
		class GpuActionArray
		{
		private:			
			int elementCount;
			karthy::cuda::GpuAction bestAction;
			karthy::cuda::GpuAction exploreAction;

			karthy::cuda::GpuAction* d_searchActionSymmetricResult;
		public:
			int* d_elementX;
			int* d_elementY;

			double* elementQValue;
			uint64_t* nextStateId;

			int taker;
			uint64_t id;

			GpuActionArray();

			void addAction(const karthy::cuda::GpuAction& actionForAdd);
			void clearAll(void);
			void print(void);

			//return an action index
			karthy::cuda::GpuAction searchActionSymmetric(const karthy::cuda::GpuAction& actionForSearch, int symmetricType);

			GpuAction getBestAction(void);
			GpuAction getExploreAction(void);
			GpuAction getActionByIndex(int index);
			
			CUDA_CALLABLE_MEMBER int getElementCount(void);
		};
	}
}