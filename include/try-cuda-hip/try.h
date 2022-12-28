#ifndef TRY-CUDA-HIP_TRY_H
#define TRY-CUDA-HIP_TRY_H

#include <stdint.h>

typedef void *Blake3Handler;

#ifdef __cplusplus
extern "C" {
#endif
/***
 * Init blake3 cuda environment. Must be called before any other blake3 cuda API.
 * @param [out] gpu_index Indexes of cuda capable GPUs.
 * @param [out] gpu_num Number of cuda capable GPUs.
 * @return true indicates success.
 */

bool query_available_gpu(int *gpu_index, int *gpu_num, int gpu_index_length);

// bool create_handler(Blake3Handler *handler,const int *gpu_index, int gpu_num);

#ifdef __cplusplus
}
#endif

#endif //TRY-CUDA-HIP_TRY_H

