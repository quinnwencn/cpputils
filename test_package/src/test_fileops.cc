#include "cpputils/fileops.h"

#include <gtest/gtest.h>

TEST(UtilsTest, ReadNonExistFile) {
    std::filesystem::path nonExistFile {"/not/exist/file"};
    auto content = Cpputils::ReadFileContent(nonExistFile);
    EXPECT_TRUE(content.empty());
}

TEST(UtilsTest, ReadNoPermissionFile) {
    // in unix
    std::filesystem::path nonExistFile {"/etc/sudoers"};
    auto content = Cpputils::ReadFileContent(nonExistFile);
    EXPECT_TRUE(content.empty());
}

TEST(UtilsTest, ReadNormalFile) {
    // in unix
    std::filesystem::path nonExistFile {TEST_MAIN_CC};
    auto content = Cpputils::ReadFileContent(nonExistFile);
    EXPECT_FALSE(content.empty());
}
