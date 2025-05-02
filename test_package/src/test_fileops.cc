#include "cpputils/fileops.h"

#include <gtest/gtest.h>

TEST(CpputilsTest, ReadNonExistFile) {
    std::filesystem::path nonExistFile {"/not/exist/file"};
    auto content = Cpputils::ReadFile(nonExistFile);
    EXPECT_TRUE(content.empty());
}

TEST(CpputilsTest, ReadNoPermissionFile) {
#if defined(__unix__) || defined(__APPLE__)
    std::filesystem::path nonExistFile {"C:/Windows/System32/xwizard.dtd"};
    auto content = Cpputils::ReadFile(nonExistFile);
    EXPECT_TRUE(content.empty());
#elif defined(_WIN32)
    std::filesystem::path nonExistFile {"/etc/sudoers"};
    auto content = Cpputils::ReadFile(nonExistFile);
    EXPECT_TRUE(content.empty());
#else
    GTEST_SKIP() << "This test is not applicable on this platform.";
#endif
}

TEST(CpputilsTest, ReadNormalFile) {
    std::filesystem::path mainCC {TEST_MAIN_CC};
    auto content = Cpputils::ReadFile(mainCC);
    EXPECT_FALSE(content.empty());
}

TEST(CpputilsTest, WriteFile) {
    std::filesystem::path testFile {std::filesystem::temp_directory_path() / "test_file.txt"};
    std::string content = "Hello, World!";
    Cpputils::WriteFile(testFile, content);
    
    auto readContent = Cpputils::ReadFile(testFile);
    EXPECT_EQ(content, readContent);
    
    std::filesystem::remove(testFile);
}
