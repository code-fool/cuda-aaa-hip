
#ifndef CXPRINT_CPP
#define CXPRINT_CPP
#include <string>
#include "cxxopts.hpp"
#include "gtest/gtest.h"
#include "glog/logging.h"
#include "global_variable.h"
uint32_t g_block_dim = 128;
uint32_t g_block_num = 420;
uint32_t g_block_size_limit = 64;
int main(int argc, char **argv) {
//Overwrite configuration of google log.
    FLAGS_logtostderr = true;
    ::testing::InitGoogleTest(&argc, argv);

    std::string CMD_GRID = "grid";
    std::string CMD_BLOCK = "block";
    std::string CMD_BLOCK_SIZE_LIMIT = "block-size-limit";

    cxxopts::Options options(argv[0], "Additional arguments for passing parameters to test unit.");
    options.allow_unrecognised_options();
    options.add_options()
        (CMD_GRID, "grid of the kernel", cxxopts::value<uint32_t>()->default_value(std::to_string(g_block_num)))
        (CMD_BLOCK, "block of the kernel", cxxopts::value<uint32_t>()->default_value(std::to_string(g_block_dim)))
        (CMD_BLOCK_SIZE_LIMIT, "block size limit(0 means no limit)", cxxopts::value<uint32_t>()->default_value(std::to_string(g_block_size_limit)))
        ("h,help", "Print help information", cxxopts::value<bool>());
        
    cxxopts::ParseResult result;
    try{
        result = options.parse(argc, argv);
    } catch (cxxopts::argument_incorrect_type ex){
        LOG(ERROR) << ex.what();
        exit(-1);
    }

    if (result.count("help") || result.count("h")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result.count(CMD_GRID)) {
        g_block_num = result[CMD_GRID].as<uint32_t>();
    }

    if (result.count(CMD_BLOCK)) {
        g_block_dim = result[CMD_BLOCK].as<uint32_t>();
    }

    if (result.count(CMD_BLOCK_SIZE_LIMIT)) {
        g_block_size_limit = result[CMD_BLOCK_SIZE_LIMIT].as<uint32_t>();
    }

    //printf("Block number:%u, block dim:%u block_size_limit:%u\n", g_block_num, g_block_dim, g_block_size_limit);
    return RUN_ALL_TESTS();
}
#endif 