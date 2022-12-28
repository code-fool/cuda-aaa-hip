#!/bin/bash
set -e
BUILD_DIR=build-release
mkdir -p $BUILD_DIR
cmake -S . -B $BUILD_DIR -DCMAKE_BUILD_TYPE=release -DBLAKE3_CUDA_BUILD_TESTS=ON
cmake --build $BUILD_DIR --clean-first
$BUILD_DIR/blake3_cuda-test --gtest_filter=*CheckResult*



