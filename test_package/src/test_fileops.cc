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
    // do nothing for windows
#else
    GTEST_SKIP() << "This test is not applicable on this platform.";
#endif
}

TEST(FileopsTest, ReadNormalFile) {
    std::filesystem::path mainCC {TEST_MAIN_CC};
    auto content = Cpputils::ReadFile(mainCC);
    EXPECT_FALSE(content.empty());

#ifdef _WIN32
    std::filesystem::path file = "./test.txt";
#else
    std::filesystem::path file = "/tmp/test.txt";
#endif
    std::string str { "Hello World\nLite 2" };
    std::ofstream ofs { file, std::ios::binary };
    ofs << str;
    ofs.flush();

	auto contentVec = Cpputils::ReadFile2Vec(file);
	std::string actual(contentVec.begin(), contentVec.end());
	EXPECT_EQ(actual, str);
}

TEST(ReadFileTest, ReadsBinaryFile) {
    std::vector<uint8_t> testData{0x01, 0x02, 0x03, 0x00, 0xFF};

    // Write to file
#ifdef _WIN32
    std::filesystem::path file = "./test.txt";
#else
    std::filesystem::path file = "/tmp/test.txt";
#endif
    std::ofstream ofs {file, std::ios::binary};
    ofs.write(
        reinterpret_cast<const char*>(testData.data()),
        testData.size()
    );
    ofs.flush();

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
