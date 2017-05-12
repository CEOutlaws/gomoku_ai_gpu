#include "karthy_GpuAction.h"

namespace karthy
{
	namespace cuda
	{
		class GpuMatrix
		{
		public:
			int* d_element;

			GpuMatrix();
			void setElement(const karthy::cuda::GpuAction& action, int taker);
			void setElement(int row, int col, int value);
			void setAllElement(int value);
			void print(void);
		};
	}
}