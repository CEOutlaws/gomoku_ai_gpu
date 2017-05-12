#pragma once
#include "../karthy_ShareCommon.h"
#include <stdio.h>

#if DEBUG
#define PRINT_ERROR() printError()
#else 
#define PRINT_ERROR()
#endif
typedef unsigned long long int uint64_t;

namespace karthy
{
	namespace cuda
	{
		//host: CPU
		//device: GPU
		//host pointer to device variable: d_pointerOfDeviceVariable
		void printError(void);
	}
}