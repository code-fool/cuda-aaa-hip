#include <cuda_runtime.h>
#include <string>
#include <regex>
#include "glog/logging.h"
#include "try-cuda-hip/try.h"
#include "cuda-helper/cuda-helper.h"
bool is_le(){
    uint32_t a = 1;
    uint8_t *ptr = (uint8_t*)&a;
    return ptr[0] == 1;
}

bool query_available_gpu(int *gpu_index, int *gpu_num, int gpu_index_length) {
    try
    {
        // get the num of cuda 
        cudaCheckError(cudaGetDeviceCount(gpu_num));
        if (*gpu_num == 0) {
            LOG(INFO) << "No available cuda capable gpus. ";
            return true;
        }

        for (int i = 0; i < std::min(*gpu_num, gpu_index_length); i++) {
            gpu_index[i] = i;
        }
        return true;
    }
    catch(std::exception &ex){
        LOG(ERROR) << "Can not get number of cuda capable gpus. Error:" << ex.what();
        return false;
    }
}
