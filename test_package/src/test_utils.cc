#include "cpputils/string.h"

#include <gtest/gtest.h>

TEST(UtilsTest, EmptyArr2StrTest) {
    std::vector<uint8_t> empty;
    EXPECT_TRUE(Utils::ByteArr2HexStr(empty).empty());
}

TEST(UtilsTest, Arr2StrTest) {
    std::vector<uint8_t> array {1, 2, 3, 4, 0xa, 0xb};
    auto str = Utils::ByteArr2HexStr(array);
    
    EXPECT_EQ(str.length(), array.size() * 2);
    EXPECT_EQ("010203040a0b", str);
}

TEST(UtilsTest, ReadNonExistFile) {
    std::filesystem::path nonExistFile {"/not/exist/file"};
    auto content = Utils::ReadFileContent(nonExistFile);
    EXPECT_TRUE(content.empty());
}

TEST(UtilsTest, ReadNoPermissionFile) {
    // in unix
    std::filesystem::path nonExistFile {"/etc/sudoers"};
    auto content = Utils::ReadFileContent(nonExistFile);
    EXPECT_TRUE(content.empty());
}

TEST(UtilsTest, ReadNormalFile) {
    // in unix
    std::filesystem::path nonExistFile {TEST_MAIN_CC};
    auto content = Utils::ReadFileContent(nonExistFile);
    EXPECT_FALSE(content.empty());
}