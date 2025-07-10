#include "cpputils/fileops.h"

#include <gtest/gtest.h>

TEST(FileopsTest, ReadNonExistFile) {
    std::filesystem::path nonExistFile {"/not/exist/file"};
    auto content = Cpputils::ReadFile(nonExistFile);
    EXPECT_TRUE(content.empty());
}

TEST(FileopsTest, ReadNoPermissionFile) {
#if defined(__unix__) || defined(__APPLE__)
    std::filesystem::path nonPermissionFile {"/etc/sudoers"};
    auto content = Cpputils::ReadFile(nonPermissionFile);
    EXPECT_TRUE(content.empty());

	auto contentVec = Cpputils::ReadFile2Vec(nonPermissionFile);
	EXPECT_TRUE(contentVec.empty());
#elif defined(_WIN32)
    std::filesystem::path nonPermissionFile {"C:/Windows/System32/xwizard.dtd"};
    auto content = Cpputils::ReadFile(nonPermissionFile);
    EXPECT_TRUE(content.empty());

	auto contentVec = Cpputils::ReadFile2Vec(nonPermissionFile);
	EXPECT_TRUE(contentVec.empty());
#else
    GTEST_SKIP() << "This test is not applicable on this platform.";
#endif
}

TEST(FileopsTest, ReadNormalFile) {
    std::filesystem::path mainCC {TEST_MAIN_CC};
    auto content = Cpputils::ReadFile(mainCC);
    EXPECT_FALSE(content.empty());

	auto contentVec = Cpputils::ReadFile2Vec(mainCC);
	EXPECT_TRUE(!contentVec.empty());
}

TEST(FileopsTest, WriteFile) {
    std::filesystem::path testFile {std::filesystem::temp_directory_path() / "test_file.txt"};
    std::string content = "Hello, World!";
    Cpputils::WriteFile(testFile, content);
    
    auto readContent = Cpputils::ReadFile(testFile);
    EXPECT_EQ(content, readContent);
    
    std::filesystem::remove(testFile);
}

TEST(FileopsTest, ExtractFileNameTest) {
    auto file = Cpputils::ExtractFileName(__FILE__);
    EXPECT_EQ(file, "test_fileops.cc");
}
