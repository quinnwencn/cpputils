#include "cpputils/fileops.h"

#include <gtest/gtest.h>
#include <fstream>

// --- ReadFile ---

TEST(ReadFile, NonExistentFileReturnsEmpty) {
    auto content = Cpputils::ReadFile("/not/exist/file");
    EXPECT_TRUE(content.empty());
}

TEST(ReadFile, NoPermissionFileReturnsEmpty) {
#if defined(__unix__) || defined(__APPLE__)
    auto content = Cpputils::ReadFile("/etc/sudoers");
    EXPECT_TRUE(content.empty());
#elif defined(_WIN32)
    GTEST_SKIP() << "Not applicable on Windows.";
#else
    GTEST_SKIP() << "Not applicable on this platform.";
#endif
}

TEST(ReadFile, ReadsKnownFile) {
    std::filesystem::path thisFile {__FILE__};
    auto content = Cpputils::ReadFile(thisFile);
    EXPECT_FALSE(content.empty());
    EXPECT_NE(std::string::npos, content.find("ReadFile"));
}

TEST(ReadFile, RoundTripWithWriteFile) {
    auto path = std::filesystem::temp_directory_path() / "cpputils_readfile_roundtrip.txt";
    std::string expected = "round trip content\nwith newlines\n";
    Cpputils::WriteFile(path, expected);

    EXPECT_EQ(expected, Cpputils::ReadFile(path));
    std::filesystem::remove(path);
}

TEST(ReadFile, EmptyFileReturnsEmpty) {
    auto path = std::filesystem::temp_directory_path() / "cpputils_readfile_empty.txt";
    Cpputils::WriteFile(path, "");

    EXPECT_TRUE(Cpputils::ReadFile(path).empty());
    std::filesystem::remove(path);
}

// --- ReadFile2Vec ---

TEST(ReadFile2Vec, NonExistentFileReturnsEmpty) {
    EXPECT_TRUE(Cpputils::ReadFile2Vec("/not/exist/file").empty());
}

TEST(ReadFile2Vec, NoPermissionFileReturnsEmpty) {
#if defined(__unix__) || defined(__APPLE__)
    EXPECT_TRUE(Cpputils::ReadFile2Vec("/etc/sudoers").empty());
#else
    GTEST_SKIP() << "Not applicable on this platform.";
#endif
}

TEST(ReadFile2Vec, EmptyFileReturnsEmpty) {
    auto path = std::filesystem::temp_directory_path() / "cpputils_readvec_empty.txt";
    Cpputils::WriteFile(path, "");

    EXPECT_TRUE(Cpputils::ReadFile2Vec(path).empty());
    std::filesystem::remove(path);
}

TEST(ReadFile2Vec, TextContent) {
    auto path = std::filesystem::temp_directory_path() / "cpputils_readvec_text.txt";
    std::string str {"Hello World\nLine 2"};
    {
        std::ofstream ofs(path, std::ios::binary);
        ofs << str;
    }

    auto vec = Cpputils::ReadFile2Vec(path);
    std::string actual(vec.begin(), vec.end());
    EXPECT_EQ(str, actual);
    std::filesystem::remove(path);
}

TEST(ReadFile2Vec, BinaryContent) {
    std::vector<uint8_t> testData {0x01, 0x02, 0x03, 0x00, 0xFF};
    auto path = std::filesystem::temp_directory_path() / "cpputils_readvec_bin.bin";
    {
        std::ofstream ofs(path, std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(testData.data()),
                  static_cast<std::streamsize>(testData.size()));
    }

    EXPECT_EQ(testData, Cpputils::ReadFile2Vec(path));
    std::filesystem::remove(path);
}

// --- WriteFile ---

TEST(WriteFile, CreatesNewFile) {
    auto path = std::filesystem::temp_directory_path() / "cpputils_writefile_new.txt";
    std::filesystem::remove(path);

    Cpputils::WriteFile(path, "new content");
    EXPECT_EQ("new content", Cpputils::ReadFile(path));
    std::filesystem::remove(path);
}

TEST(WriteFile, OverwritesExistingFile) {
    auto path = std::filesystem::temp_directory_path() / "cpputils_writefile_overwrite.txt";
    Cpputils::WriteFile(path, "first");
    Cpputils::WriteFile(path, "second");

    EXPECT_EQ("second", Cpputils::ReadFile(path));
    std::filesystem::remove(path);
}

TEST(WriteFile, WritesEmptyString) {
    auto path = std::filesystem::temp_directory_path() / "cpputils_writefile_empty.txt";
    Cpputils::WriteFile(path, "");

    EXPECT_TRUE(Cpputils::ReadFile(path).empty());
    std::filesystem::remove(path);
}

// --- ExtractFileName ---

TEST(ExtractFileName, FromCurrentFile) {
    EXPECT_EQ("test_fileops.cc", Cpputils::ExtractFileName(__FILE__));
}

TEST(ExtractFileName, UnixPath) {
    EXPECT_EQ("file.txt", Cpputils::ExtractFileName("/home/user/file.txt"));
}

TEST(ExtractFileName, WindowsPath) {
    EXPECT_EQ("file.txt", Cpputils::ExtractFileName("C:\\Users\\user\\file.txt"));
}

TEST(ExtractFileName, BareFilename) {
    EXPECT_EQ("file.txt", Cpputils::ExtractFileName("file.txt"));
}

TEST(ExtractFileName, TrailingSlash) {
    EXPECT_EQ("", Cpputils::ExtractFileName("/home/user/"));
}

TEST(ExtractFileName, NoExtension) {
    EXPECT_EQ("Makefile", Cpputils::ExtractFileName("/build/Makefile"));
}

TEST(ExtractFileName, DotFile) {
    EXPECT_EQ(".gitignore", Cpputils::ExtractFileName("/repo/.gitignore"));
}

// --- TemporaryFile ---

TEST(TemporaryFile, PathContainsHint) {
    Cpputils::TemporaryFile tmp("cpputils_test_hint.txt");
    auto path = tmp.Path();
    EXPECT_NE(std::string::npos, path.string().find("cpputils_test_hint.txt"));
}

TEST(TemporaryFile, PathIsInTempDir) {
    Cpputils::TemporaryFile tmp("cpputils_tmpfile_loc.txt");
    auto tmpDir = std::filesystem::temp_directory_path().string();
    EXPECT_EQ(0u, tmp.Path().string().find(tmpDir));
}

TEST(TemporaryFile, DestructorRemovesFile) {
    std::filesystem::path path;
    {
        Cpputils::TemporaryFile tmp("cpputils_tmpfile_dtor.txt");
        tmp.Write("data");
        path = tmp.Path();
        EXPECT_TRUE(std::filesystem::exists(path));
    }
    EXPECT_FALSE(std::filesystem::exists(path));
}

TEST(TemporaryFile, DestructorSafeOnNonExistentFile) {
    Cpputils::TemporaryFile tmp("cpputils_tmpfile_no_write.txt");
    std::filesystem::remove(tmp.Path());
}

TEST(TemporaryFile, WriteString) {
    Cpputils::TemporaryFile tmp("cpputils_tmpfile_write_str.txt");
    tmp.Write("hello world");

    EXPECT_EQ("hello world", Cpputils::ReadFile(tmp.Path()));
}

TEST(TemporaryFile, WriteOverwrites) {
    Cpputils::TemporaryFile tmp("cpputils_tmpfile_write_over.txt");
    tmp.Write("first");
    tmp.Write("second");

    EXPECT_EQ("second", Cpputils::ReadFile(tmp.Path()));
}

TEST(TemporaryFile, WriteBinary) {
    std::vector<uint8_t> data {0xDE, 0xAD, 0xBE, 0xEF};
    Cpputils::TemporaryFile tmp("cpputils_tmpfile_write_bin.bin");
    tmp.Write(data);

    EXPECT_EQ(data, Cpputils::ReadFile2Vec(tmp.Path()));
}

TEST(TemporaryFile, WriteBinaryOverwrites) {
    Cpputils::TemporaryFile tmp("cpputils_tmpfile_wbin_over.bin");
    tmp.Write(std::vector<uint8_t>{0x01, 0x02, 0x03});
    tmp.Write(std::vector<uint8_t>{0xAA, 0xBB});

    std::vector<uint8_t> expected {0xAA, 0xBB};
    EXPECT_EQ(expected, Cpputils::ReadFile2Vec(tmp.Path()));
}

TEST(TemporaryFile, AppendString) {
    Cpputils::TemporaryFile tmp("cpputils_tmpfile_append_str.txt");
    tmp.Write("hello");
    tmp.Append(" world");

    EXPECT_EQ("hello world", Cpputils::ReadFile(tmp.Path()));
}

TEST(TemporaryFile, AppendMultiple) {
    Cpputils::TemporaryFile tmp("cpputils_tmpfile_append_multi.txt");
    tmp.Append("a");
    tmp.Append("b");
    tmp.Append("c");

    EXPECT_EQ("abc", Cpputils::ReadFile(tmp.Path()));
}

TEST(TemporaryFile, AppendBinary) {
    Cpputils::TemporaryFile tmp("cpputils_tmpfile_append_bin.bin");
    tmp.Write(std::vector<uint8_t>{0x01, 0x02});
    tmp.Append(std::vector<uint8_t>{0x03, 0x04});

    std::vector<uint8_t> expected {0x01, 0x02, 0x03, 0x04};
    EXPECT_EQ(expected, Cpputils::ReadFile2Vec(tmp.Path()));
}

TEST(TemporaryFile, AppendBinaryMultiple) {
    Cpputils::TemporaryFile tmp("cpputils_tmpfile_abin_multi.bin");
    tmp.Append(std::vector<uint8_t>{0xAA});
    tmp.Append(std::vector<uint8_t>{0xBB});
    tmp.Append(std::vector<uint8_t>{0xCC});

    std::vector<uint8_t> expected {0xAA, 0xBB, 0xCC};
    EXPECT_EQ(expected, Cpputils::ReadFile2Vec(tmp.Path()));
}

TEST(TemporaryFile, MixStringAndBinaryAppend) {
    Cpputils::TemporaryFile tmp("cpputils_tmpfile_mix.bin");
    tmp.Write("AB");
    tmp.Append(std::vector<uint8_t>{0x43, 0x44});

    auto content = Cpputils::ReadFile2Vec(tmp.Path());
    std::vector<uint8_t> expected {'A', 'B', 0x43, 0x44};
    EXPECT_EQ(expected, content);
}
