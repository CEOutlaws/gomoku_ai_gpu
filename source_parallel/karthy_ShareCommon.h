#pragma once
#define DEBUG 0

#define TIC_TAC_TOE 0

#if TIC_TAC_TOE
#define BOARD_SIDE_LENGTH (3)
#define STONES_TO_END_GAME (3)
#else
#define BOARD_SIDE_LENGTH (11)
#define STONES_TO_END_GAME (5)
#endif

#ifdef __CUDACC__
#define CUDA_CALLABLE_MEMBER __host__ __device__
#else
#define CUDA_CALLABLE_MEMBER
#endif 