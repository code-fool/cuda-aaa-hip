cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(blake3_cuda LANGUAGES CXX CUDA)

if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE RELEASE)
endif()
string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckLanguage)
check_language(CUDA)
set(CMAKE_CUDA_STANDARD 11)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

include(FetchContent)
if(NOT CUDAToolkit_FOUND)
        include(FindCUDAToolkit)
endif()
include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
if(WIN32)
        include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}/extras/CUPTI/include)
        link_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}/extras/CUPTI/lib64)
endif()
if(${CMAKE_BUILD_TYPE} STREQUAL "RELEASE")
        set(NVCC_FLAGS -O3)
        message(STATUS "Compile cuda code in release mode")
else()
        set(NVCC_FLAGS -G)
        message(STATUS "Compile cuda code in debug mode")
endif()

#Google log library
option (WITH_CUSTOM_PREFIX "" ON)
option (BUILD_SHARED_LIBS "" OFF)
option (WITH_GFLAGS "" OFF)
option (WITH_GTEST "" OFF)
option (WITH_PKGCONFIG "" OFF)
option (WITH_SYMBOLIZE "" OFF)
option (WITH_UNWIND "" OFF)
set(BUILD_TESTING OFF CACHE BOOL "Build unit test")
FetchContent_Declare(
        glog
        GIT_REPOSITORY https://github.com/google/glog.git
        GIT_TAG 8f9ccfe770add9e4c64e9b25c102658e3c763b73 #release 0.5
)
FetchContent_MakeAvailable(glog)
include_directories(${glog_SOURCE_DIR}/include)
link_directories(${glog_BINARY_DIR})

#Cuda helper
FetchContent_Declare(
        cuda_helper
        # GIT_REPOSITORY https://github.com/6block/cuda-helper.git
        GIT_REPOSITORY https://github.com/code-fool/cuda-helper.git
        GIT_TAG main
        # GIT_TAG v2.2.1
)
FetchContent_MakeAvailable(cuda_helper)
include_directories(${cuda_helper_SOURCE_DIR}/include)
link_directories(${cuda_helper_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE source_file LIST_DIRECTORIES false  src/*.cu src/*.cuh src/*.cpp)

add_library(${PROJECT_NAME} STATIC ${source_file} )

target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:
        ${NVCC_FLAGS} -Wno-deprecated-gpu-targets
        >)

set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_ARCHITECTURES "all")


target_link_libraries(${PROJECT_NAME}
                        PRIVATE CUDA::cudart
                        PRIVATE glog
                        PRIVATE cuda-helper)

#set_target_properties(${PROJECT_NAME} PROPERTIES
#                        ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)


string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER_CASE)
set(BUILD_TEST_OPTION ${PROJECT_NAME_UPPER_CASE}_BUILD_TESTS)
option(${BUILD_TEST_OPTION} "Enable building google test units" OFF)

if(${BUILD_TEST_OPTION})
        #Google Unit Test
        option(BUILD_GMOCK "" OFF)
        option(INSTALL_GTEST "" OFF)
        FetchContent_Declare(
                googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG main
        )
        FetchContent_MakeAvailable(googletest)

        #cxxopts
        FetchContent_Declare(
                cxxopts
                GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
                GIT_TAG c74846a891b3cc3bfa992d588b1295f528d43039 #v3.0.0
        )
        FetchContent_MakeAvailable(cxxopts)

        # FetchContent_Declare(
        #         blake3-baseline
        #         GIT_REPOSITORY  https://github.com/6block/blake3-baseline.git
        # )
        # FetchContent_MakeAvailable(blake3-baseline)

        file(GLOB_RECURSE test_files LIST_DIRECTORIES false
                include/*.h include/*.cuh include/*.hpp
                test/*.h test/*.cuh test/*.hpp
                src/*.h src/*.cuh src/*.hpp
                test/*.cpp test/*.cu
                src/*.cpp src/*.cu
                )

        set(TEST_EXE ${PROJECT_NAME}-test)
        add_executable(${TEST_EXE} ${test_files})
        set_target_properties(${TEST_EXE} PROPERTIES CUDA_ARCHITECTURES "native")
        target_include_directories(${TEST_EXE} PRIVATE ${cxxopts_SOURCE_DIR}/include)
        target_include_directories(${TEST_EXE} PRIVATE ${googletest_SOURCE_DIR}/include)
        # target_include_directories(${TEST_EXE} PRIVATE ${blake3-baseline_SOURCE_DIR}/include)
        target_include_directories(${TEST_EXE} PRIVATE ${PROJECT_SOURCE_DIR}/src)
        target_link_directories(${TEST_EXE} PRIVATE ${cxxopts_BINARY_DIR})
        target_link_directories(${TEST_EXE} PRIVATE ${googletest_BINARY_DIR})
        # target_link_directories(${TEST_EXE} PRIVATE ${blake3-baseline_BINARY_DIR})

        target_link_libraries(${TEST_EXE}
                PRIVATE gtest
                PRIVATE GTest::gtest_main
                PRIVATE CUDA::cudart
                # PRIVATE blake3-baseline
                PRIVATE cxxopts
                PRIVATE cuda-helper
                PRIVATE glog)

        target_compile_options(${TEST_EXE} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:
                ${NVCC_FLAGS}
                >)

        include(GoogleTest)
        gtest_discover_tests(${TEST_EXE})
endif()

if(APPLE)
    message(FATAL_ERROR "Unsupported on mac")
endif()

