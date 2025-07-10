#pragma once 

#include <string>
#include <filesystem>
#include <string_view>
#include <vector>

namespace Cpputils {

std::string ReadFile(const std::filesystem::path& file);
std::vector<uint8_t> ReadFile2Vec(const std::filesystem::path& file);
void WriteFile(const std::filesystem::path& file, std::string_view content);
std::string ExtractFileName(std::string_view filename);

class TemporaryFile {
public:
    explicit TemporaryFile(std::string_view hint);
    ~TemporaryFile();
    TemporaryFile(const TemporaryFile&) = delete;
    TemporaryFile& operator=(const TemporaryFile&) = delete;
    TemporaryFile(TemporaryFile&&) = delete;
    TemporaryFile& operator=(TemporaryFile&&) = delete;

    void Append(std::string_view content);
    void Append(const std::vector<uint8_t>& content);

    void Write(std::string_view content);
    void Write(const std::vector<uint8_t>& content);

    std::filesystem::path Path() const { return TemporaryFilePath_; }

private:
    std::filesystem::path TemporaryFilePath_;
};

}