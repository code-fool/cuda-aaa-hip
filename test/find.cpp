
#include <thread>
#include <math.h>
#include <algorithm>
#include "gtest/gtest.h"
#include "cuda-helper/cuda-helper.h"




TEST(Blake3Calculator, CheckResult) {
    int count;
    printf("CheckResult！！！！！！！！！\n");
    cudaCheckError(cudaGetDeviceCount(&count));
}