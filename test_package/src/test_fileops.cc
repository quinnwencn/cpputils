#include "cpputils/fileops.h"

#include <gtest/gtest.h>
#include <fstream>

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

	std::filesystem::path file = "/tmp/test.txt";
	std::string str { "Hello World\nLite 2" };
	std::ofstream(file) << str;

	auto contentVec = Cpputils::ReadFile2Vec(file);
	std::string actual(contentVec.begin(), contentVec.end());
	EXPECT_EQ(actual, str);
}

TEST(ReadFileTest, ReadsBinaryFile) {
    std::vector<uint8_t> testData{0x01, 0x02, 0x03, 0x00, 0xFF};

    // Write to file
    std::filesystem::path file = "/tmp/test.bin";
    std::ofstream(file, std::ios::binary).write(
        reinterpret_cast<const char*>(testData.data()),
        testData.size()
    );

    // Test the function
    auto content = Cpputils::ReadFile2Vec(file);

    // Verify
    EXPECT_EQ(testData, content);
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
