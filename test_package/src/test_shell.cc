//
// Created by quinn on 7/19/2025.
// Copyright (c) 2025 All rights reserved.
//

#include "cpputils/shell.h"

#include <string>
#include <gtest/gtest.h>

TEST(ShellTest, SimpleTest) {
    std::string cmd {"ls"};
    EXPECT_EQ(0, Cpputils::Shell(cmd));
}

TEST(ShellTest, OutputTest) {
    std::string cmd {"ls -alh ."};
    std::string output;

    EXPECT_EQ(0, Cpputils::Shell(cmd, output));
    EXPECT_TRUE(output.find("total") != std::string::npos);
}

TEST(ShellTest, OutputWithStderrTest) {
    std::string cmd {"tdd -alh ."};
    std::string output;

    EXPECT_NE(0, Cpputils::Shell(cmd, output, true));
    EXPECT_TRUE(output.find("not found") != std::string::npos);
}
